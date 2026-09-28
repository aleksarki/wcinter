#define UNICODE
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <algorithm>
#include <cmath>
#include <ranges>
#include <list>
#include <stdexcept>
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
    wci::CharMatrix framebuf;  // framebuf is used to forgo allocating matrices anew each rendering
    wci::Handle oldScreenBuffer;
    std::vector<std::pair<std::size_t, wci::Widget*>> widgets;  // todo make sth abt it
    std::list<wci::Widget> orphans;  // list because it does not invalidate pointers on relocation and deletion
    std::size_t widgetCounter = 0;

public:
    Impl() : inner{ wci::Console(), wci::CharMatrix(0, 0) }, framebuf()
    {
        inner.matrix.resize(inner.console.screenBufferInfo().size);
        oldScreenBuffer = inner.console.activeScreenBuffer();
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
        inner.console.activeScreenBuffer(oldScreenBuffer);
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
        wci::Coord anchorPoint{ 0, 0 }, startPoint{ 0, 0 };  // ffs just in case

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
        if (matrix().empty())
            return;

        if (framebuf.size() != size())  
            framebuf.resize(size());
        std::copy(matrix().data(), matrix().data() + size().x * size().y, framebuf.data());

        for (const auto& [id, widget] : widgets)
        {
            wci::Coord startPoint = calculateWidgetStart(
                widget->position().x, widget->position().y, widget->size(), widget->anchor()
            );
            widget->absolute(startPoint);
            framebuf.inlay(widget->absolute(), widget->render());
        }

        SMALL_RECT rect{
            0, 0,
            wci::api(size().x) - 1,
            wci::api(size().y) - 1
        };
        WriteConsoleOutputW(
            wci::api(console().stdOutput()),  // same as console().activeScreenBuffer()
            wci::api(framebuf.data()),
            wci::api(framebuf.size()),
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
        this->matrix().inlay(x, y, matrix);
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

    Widget& widget(std::size_t id)
    {
        auto it = std::ranges::find(widgets, id, &std::pair<size_t, wci::Widget*>::first);
        if (it == widgets.end())
            throw std::invalid_argument("Such widget does not exist");
        return *it->second;
    }

    std::size_t placeWidget(const wci::LenPercent& x, const wci::LenPercent& y, wci::Widget& widget, wci::Anchor anchor)
    {
        widget.position(wci::PositionSpec{ x, y });
        widget.anchor(anchor);
        widget.absolute(calculateWidgetStart(x, y, widget.size(), anchor));

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
        orphans.push_back(std::move(widget));
        return placeWidget(x, y, orphans.back(), anchor);
    }
    std::size_t placeWidget(const wci::LenPercent& x, const wci::LenPercent& y, std::size_t id, wci::Anchor anchor)
    {
        auto& widget = this->widget(id);
        widget.position(wci::PositionSpec{ x, y });
        widget.anchor(anchor);
        widget.absolute(calculateWidgetStart(x, y, widget.size(), anchor));
        return id;
    }

    void removeIfOrphan(wci::Widget* orphan)
    {
        std::erase_if(orphans, [orphan](const wci::Widget& widget) -> bool { return &widget == orphan; });
    }

    void unplaceWidget(std::size_t id)
    {
        bool found = false;
        std::erase_if(widgets, [this, id, &found](const std::pair<size_t, wci::Widget*>& pair) -> bool
        {
            if (pair.first != id)
                return false;
            found = true;
            pair.second->placed(false);
            removeIfOrphan(pair.second);
            return true;
        });
        if (!found)
            throw std::invalid_argument("Such widget does not exist");
    }
    void unplaceWidget(wci::Widget& widget)
    {
        bool found = false;
        std::erase_if(widgets, [this, &widget, &found](const std::pair<size_t, wci::Widget*>& pair) -> bool
        {
            if (pair.second != &widget)
                return false;
            found = true;
            pair.second->placed(false);
            removeIfOrphan(pair.second);
            return true;
        });
        if (!found)
            throw std::invalid_argument("Such widget does not exist");
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
std::size_t wci::Window::place(const LenPercent& x, const LenPercent& y, std::size_t id, Anchor anchor)
{
    return impl->placeWidget(x, y, id, anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, wci::Widget& widget, wci::Anchor anchor)
{
    return impl->placeWidget(position.x, position.y, widget, anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, wci::Widget&& widget, wci::Anchor anchor)
{
    return impl->placeWidget(position.x, position.y, std::move(widget), anchor);
}
std::size_t wci::Window::place(const PositionSpec& position, std::size_t id, Anchor anchor)
{
    return impl->placeWidget(position.x, position.y, id, anchor);
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

wci::Widget& wci::Window::widget(std::size_t id)
{
    return impl->widget(id);
}
const wci::Widget& wci::Window::widget(std::size_t id) const
{
    return impl->widget(id);
}
