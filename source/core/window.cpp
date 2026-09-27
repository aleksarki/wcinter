#define UNICODE
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <deque>
#include <ranges>
#include <utility>
#include <vector>
#include <windows.h>

#include "window.hpp"
#include "../defs/apicast.hpp"

class wci::Window::Impl
{
private:
    struct {
        wci::Console console;
        wci::CharMatrix matrix;
    } inner;
    struct {  // setting to be restored on destruction
        wci::Handle screenBuffer;
    } old;
    wci::CharMatrix frameBuffer;
    std::vector<std::pair<std::size_t, wci::Widget*>> widgets;  // todo make sth abt it
    std::deque<wci::Widget> orphanWidgets;  // todo switch to list  // deque because it does not invalidate pointers on relocation
    std::size_t widgetCounter = 0;

public:
    Impl() : inner{ wci::Console(), wci::CharMatrix(0, 0) }
    {
        inner.matrix.resize(inner.console.screenBufferInfo().size);
        old.screenBuffer = inner.console.activeScreenBuffer();
        HANDLE handle = CreateConsoleScreenBuffer(
            wci::api(GenericRights::Read | GenericRights::Write),
            wci::api(FileAccessRights::ShareRead | FileAccessRights::ShareWrite),
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL
        );
        inner.console.activeScreenBuffer(wci::wci(handle));
    }
    ~Impl()
    {
        inner.console.activeScreenBuffer(old.screenBuffer);
    }

    wci::Console& console() noexcept
    {
        return inner.console;
    }
    const wci::Console& console() const noexcept
    {
        return inner.console;
    }

    wci::CharMatrix& matrix() noexcept
    {
        return inner.matrix;
    }
    const wci::CharMatrix& matrix() const noexcept
    {
        return inner.matrix;
    }

    wci::Coord size() const
    {
        return matrix().size();
    }

    void resize()  // idea optimize this
    {
        auto newSize = console().screenBufferInfo().size;
        if (newSize == size())
            return;
        wci::CharMatrix newMatrix(newSize);
        newMatrix.inlay(matrix());
        matrix().swap(newMatrix);
    }

    wci::Coord calculateWidgetStart(
        const wci::LenPercent& x, const wci::LenPercent& y, wci::Coord widgetSize, wci::Anchor anchor
    ) const
    {
        wci::Coord anchorPoint, startPoint;

        if (x.isAbsolute())
            anchorPoint.x = x.absolute();
        else if (x.isRelative())
            anchorPoint.x = static_cast<wci::Short>(std::lround(x.relative() * size().x));

        if (y.isAbsolute())
            anchorPoint.y = y.absolute();
        else if (y.isRelative())
            anchorPoint.y = static_cast<wci::Short>(std::lround(y.relative() * size().y));

        switch (anchor)
        {
        case wci::Anchor::TopLeft:
            startPoint = anchorPoint;
            break;
        case wci::Anchor::Top:
            startPoint.x = anchorPoint.x - widgetSize.x / 2;
            startPoint.y = anchorPoint.y;
            break;
        case wci::Anchor::TopRight:
            startPoint.x = anchorPoint.x - widgetSize.x;
            startPoint.y = anchorPoint.y;
            break;
        case wci::Anchor::Right:
            startPoint.x = anchorPoint.x - widgetSize.x;
            startPoint.y = anchorPoint.y - widgetSize.y / 2;
            break;
        case wci::Anchor::BottomRight:
            startPoint.x = anchorPoint.x - widgetSize.x;
            startPoint.y = anchorPoint.y - widgetSize.y;
            break;
        case wci::Anchor::Bottom:
            startPoint.x = anchorPoint.x - widgetSize.x / 2;
            startPoint.y = anchorPoint.y - widgetSize.y;
            break;
        case wci::Anchor::BottomLeft:
            startPoint.x = anchorPoint.x;
            startPoint.y = anchorPoint.y - widgetSize.y;
            break;
        case wci::Anchor::Left:
            startPoint.x = anchorPoint.x;
            startPoint.y = anchorPoint.y - widgetSize.y / 2;
            break;
        case wci::Anchor::Center:
            startPoint.x = anchorPoint.x - widgetSize.x / 2;
            startPoint.y = anchorPoint.y - widgetSize.y / 2;
            break;
        }

        return startPoint;
    }

    void render()
    {
        resize();
        if (frameBuffer.size() != size())
            frameBuffer.resize(size());
        std::copy(matrix().data(), matrix().data() + size().x * size().y, frameBuffer.data());

        for (const auto& [id, widget] : widgets)
        {
            wci::Coord widgetSize = widget->size();
            wci::Coord startPoint = calculateWidgetStart(
                widget->position().x, widget->position().y, widgetSize, widget->anchor()
            );   
            widget->absolute(startPoint);
            frameBuffer.inlay(widget->absolute(), widget->render());
        }

        SMALL_RECT rect{
            0, 0,
            wci::api(size().x) - 1,
            wci::api(size().y) - 1
        };
        WriteConsoleOutputW(
            wci::api(console().stdOutput()),  // check or console().activeScreenBuffer()?
            wci::api(frameBuffer.data()),
            wci::api(frameBuffer.size()),
            COORD{ 0, 0 },
            &rect
        );
    }

    void putString(wci::Short x, wci::Short y, const wci::Wchar* string, wci::Attribute attributes)
    {
        wci::Short i = 0;
        while (string[i])
        {
            matrix().put(x + i, y, string[i], attributes);
            ++i;
        }
    }
    void putString(wci::Short x, wci::Short y, const wci::CharInfo charInfos[], size_t length)
    {
        for (size_t i = 0; i < length; ++i)
            matrix().put(x + static_cast<wci::Short>(i), y, charInfos[i]);
    }

    void putMatrix(wci::Short x, wci::Short y, const wci::CharMatrix& matrix)
    {
        inner.matrix.inlay(x, y, matrix);
    }

    bool widgetPlaced(std::size_t id) const noexcept
    {
        return std::ranges::any_of(widgets, [id](const std::pair<size_t, wci::Widget*>& pair) -> bool
        {
            return pair.first == id;
        });
    }
    bool widgetPlaced(const wci::Widget& widget) const noexcept
    {
        return std::ranges::any_of(widgets, [&widget](const std::pair<size_t, wci::Widget*>& pair) -> bool
        {
            return pair.second == &widget;
        });
    }

    std::size_t placeWidget(const wci::LenPercent& x, const wci::LenPercent& y, wci::Widget& widget, wci::Anchor anchor)
    {
        wci::Coord widgetSize = widget.size();
        wci::Coord startPoint = calculateWidgetStart(x, y, widgetSize, anchor);

        widget.position(wci::PositionSpec{ x, y });
        widget.anchor(anchor);
        widget.absolute(startPoint);

        for (const auto& [id, placed] : widgets)
            if (placed == &widget)  // this widget has already been placed
                return id;

        auto id = widgetCounter++;
        widgets.push_back(std::make_pair(id, &widget));
        widget.placed(true);
        return id;
    }
    std::size_t placeWidget(const wci::LenPercent& x, const wci::LenPercent& y, wci::Widget&& widget, wci::Anchor anchor)
    {
        orphanWidgets.push_back(std::move(widget));
        return placeWidget(x, y, orphanWidgets.back(), anchor);
    }

    void unplaceWidget(std::size_t id)
    {
        std::erase_if(widgets, [id](const std::pair<size_t, wci::Widget*>& pair) -> bool
        {
            bool found = pair.first == id;
            if (found)
                pair.second->placed(false);
            return found;
        });
    }
    void unplaceWidget(wci::Widget& widget)
    {
        std::erase_if(widgets, [&widget](const std::pair<size_t, wci::Widget*>& pair) -> bool
        {
            bool found = pair.second == &widget;
            if (found)
                pair.second->placed(false);
            return found;
        });
    }
};

wci::Window::Window() : impl(std::make_unique<Impl>()) {}
wci::Window::~Window() = default;

wci::Console& wci::Window::console() noexcept
{
    return impl->console();
}
const wci::Console& wci::Window::console() const noexcept
{
    return impl->console();
}

wci::CharMatrix& wci::Window::matrix() noexcept
{
    return impl->matrix();
}
const wci::CharMatrix& wci::Window::matrix() const noexcept
{
    return impl->matrix();
}

wci::Coord wci::Window::size() const
{
    return impl->size();
}

void wci::Window::resize()
{
    impl->resize();
}

void wci::Window::render()
{
    impl->render();
}

void wci::Window::put(wci::Short x, wci::Short y, wci::Wchar character)
{
    auto attributes = impl->console().screenBufferInfo().attributes;
    impl->matrix().put(x, y, character, attributes);
}
void wci::Window::put(wci::Short x, wci::Short y, wci::Wchar character, wci::Attribute attributes)
{
    impl->matrix().put(x, y, character, attributes);
}
void wci::Window::put(const wci::Coord& position, const wci::CharInfo& charInfo)
{
    impl->matrix().put(position, charInfo);
}
void wci::Window::put(wci::Short x, wci::Short y, const wci::Wchar* string)
{
    auto attributes = impl->console().screenBufferInfo().attributes;
    impl->putString(x, y, string, attributes);
}
void wci::Window::put(wci::Short x, wci::Short y, const wci::Wchar* string, wci::Attribute attributes)
{
    impl->putString(x, y, string, attributes);
}
void wci::Window::put(const wci::Coord& position, const wci::CharInfo charInfos[], size_t length)
{
    impl->putString(position.x, position.y, charInfos, length);
}
void wci::Window::put(wci::Short x, wci::Short y, const wci::CharMatrix& matrix)
{
    impl->putMatrix(x, y, matrix);
}
void wci::Window::put(const wci::Coord& position, const wci::CharMatrix& matrix)
{
    impl->putMatrix(position.x, position.y, matrix);
}

std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, wci::Widget& widget, wci::Anchor anchor)
{
    return impl->placeWidget(x, y, widget, anchor);
}
std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, wci::Widget&& widget, wci::Anchor anchor)
{
    return impl->placeWidget(x, y, std::move(widget), anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, wci::Widget& widget, wci::Anchor anchor)
{
    return impl->placeWidget(position.x, position.y, widget, anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, wci::Widget&& widget, wci::Anchor anchor)
{
    return impl->placeWidget(position.x, position.y, std::move(widget), anchor);
}

void wci::Window::unplace(std::size_t id)
{
    impl->unplaceWidget(id);
}
void wci::Window::unplace(wci::Widget& widget)
{
    impl->unplaceWidget(widget);
}

bool wci::Window::placed(std::size_t id) const noexcept
{
    return impl->widgetPlaced(id);
}
bool wci::Window::placed(const Widget& widget) const noexcept
{
    return impl->widgetPlaced(widget);
}
