#define UNICODE
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <algorithm>
#include <cmath>
#include <ranges>
#include <list>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <windows.h>

#include "window.hpp"
#include "../defs/apicast.hpp"

class wci::Window::Impl
{
private:
    struct WidgetPlacement
    {
        wci::Widget* widget;
        std::vector<wci::Widget*> children;
    };

    struct {
        wci::Console console;
        wci::CharMatrix matrix;
    } inner;
    wci::Handle oldScreenBuffer;
    wci::CharMatrix framebuf;                                     // framebuf is used to forgo allocating matrices anew each rendering
    std::unordered_map<std::size_t, WidgetPlacement> placements;  // id -> widget placement info
    std::unordered_map<wci::Widget*, std::size_t> ids;            // widget -> id
    std::vector<wci::Widget*> roots;                              // widgets placed directly on the window
    std::list<wci::Widget> orphans;                               // list because it does not invalidate pointers on relocation and deletion
    std::size_t counter = 0;

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

private:
    wci::Coord calculateStackeeStart(
        const wci::PositionSpec& stackeePosition, wci::Coord baseSize, wci::Coord stackeeSize, wci::Anchor stackeeAnchor
    ) const noexcept  // todo move to utils file
    {
        wci::Coord anchorPoint{ 0, 0 }, startPoint{ 0, 0 };  // ffs just in case

        if (stackeePosition.x.isAbsolute())
            anchorPoint.x = stackeePosition.x.absolute();
        else if (stackeePosition.x.isRelative())
            anchorPoint.x = static_cast<wci::Short>(std::lround(stackeePosition.x.relative() * baseSize.x));

        if (stackeePosition.y.isAbsolute())
            anchorPoint.y = stackeePosition.y.absolute();
        else if (stackeePosition.y.isRelative())
            anchorPoint.y = static_cast<wci::Short>(std::lround(stackeePosition.y.relative() * baseSize.y));

        switch (stackeeAnchor)
        {
        case wci::Anchor::TopLeft:
            startPoint = anchorPoint;
            break;
        case wci::Anchor::Top:
            startPoint.x = anchorPoint.x - stackeeSize.x / 2;
            startPoint.y = anchorPoint.y;
            break;
        case wci::Anchor::TopRight:
            startPoint.x = anchorPoint.x - stackeeSize.x;
            startPoint.y = anchorPoint.y;
            break;
        case wci::Anchor::Right:
            startPoint.x = anchorPoint.x - stackeeSize.x;
            startPoint.y = anchorPoint.y - stackeeSize.y / 2;
            break;
        case wci::Anchor::BottomRight:
            startPoint.x = anchorPoint.x - stackeeSize.x;
            startPoint.y = anchorPoint.y - stackeeSize.y;
            break;
        case wci::Anchor::Bottom:
            startPoint.x = anchorPoint.x - stackeeSize.x / 2;
            startPoint.y = anchorPoint.y - stackeeSize.y;
            break;
        case wci::Anchor::BottomLeft:
            startPoint.x = anchorPoint.x;
            startPoint.y = anchorPoint.y - stackeeSize.y;
            break;
        case wci::Anchor::Left:
            startPoint.x = anchorPoint.x;
            startPoint.y = anchorPoint.y - stackeeSize.y / 2;
            break;
        case wci::Anchor::Center:
            startPoint.x = anchorPoint.x - stackeeSize.x / 2;
            startPoint.y = anchorPoint.y - stackeeSize.y / 2;
            break;
        }

        return startPoint;
    }

    wci::CharMatrix renderPlacedWidget(const WidgetPlacement& placed) const
    {
        auto matrix = placed.widget->render();
        for (auto* child : placed.children)
        {
            if (!child->visible())
                continue;
            auto& childPlaced = placements.at(ids.at(child));
            wci::Coord startPoint = calculateStackeeStart(
                child->position(), placed.widget->size(), child->size(), child->anchor()
            );
            child->absolute(startPoint);
            matrix.inlay(startPoint, renderPlacedWidget(childPlaced));
        }
        return matrix;
    }

public:
    void render()
    {
        resize();
        if (matrix().empty())
            return;

        if (framebuf.size() != size())  
            framebuf.resize(size());
        std::copy(matrix().data(), matrix().data() + size().x * size().y, framebuf.data());

        for (auto* root : roots)
        {
            auto& placed = placements.at(ids.at(root));
            auto* widget = placed.widget;
            if (!widget->visible())
                continue;
            wci::Coord startPoint = calculateStackeeStart(
                widget->position(), size(), widget->size(), widget->anchor()
            );
            widget->absolute(startPoint);
            framebuf.inlay(startPoint, renderPlacedWidget(placed));
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
        return placements.contains(id);
    }
    bool widgetPlaced(const wci::Widget& widget) const noexcept
    {
        return ids.contains(&const_cast<wci::Widget&>(widget));
    }

    wci::Widget& getWidget(std::size_t id)
    {
        return *placements.at(id).widget;
    }
    const wci::Widget& getWidget(std::size_t id) const
    {
        return *placements.at(id).widget;
    }

    std::size_t getId(const wci::Widget& widget) const
    {
        return ids.at(&const_cast<wci::Widget&>(widget));
    }

    // for maybe new widgets; placement on window directly
    std::size_t placeWidget(const wci::PositionSpec& position, wci::Widget& widget, wci::Anchor anchor, wci::Window* window)
    {
        widget.position(position);
        widget.anchor(anchor);
        widget.absolute(calculateStackeeStart(position, size(), widget.size(), anchor));

        if (ids.contains(&widget))  // this widget has already been placed
        {
            if (widget.parentIsWidget())  // widget has been previously placed on another widget
            {
                auto& parentPlaced = placements.at(getId(widget.parent()));
                std::erase(parentPlaced.children, &widget);
                roots.push_back(&widget);
                widget.window(window);
            }
            return ids.at(&widget);
        }

        // new widget
        auto id = counter++;
        placements[id] = WidgetPlacement{ &widget, std::vector<wci::Widget*>() };
        ids[&widget] = id;
        roots.push_back(&widget);
        widget.window(window);
        return id;
    }
    // for new widgets; placement on window directly
    std::size_t placeWidget(const wci::PositionSpec& position, wci::Widget&& widget, wci::Anchor anchor, wci::Window* window)
    {
        orphans.push_back(std::move(widget));
        return placeWidget(position, orphans.back(), anchor, window);
    }
    // for already placed widgets; placement on window directly
    std::size_t placeWidget(const wci::PositionSpec& position, std::size_t id, wci::Anchor anchor, wci::Window* window)
    {
        auto& widget = getWidget(id);
        widget.position(position);
        widget.anchor(anchor);
        widget.absolute(calculateStackeeStart(position, size(), widget.size(), anchor));
    
        if (widget.parentIsWidget())  // widget has been previously placed on another widget
        {
            auto& parentPlaced = placements.at(getId(widget.parent()));
            std::erase(parentPlaced.children, &widget);
            roots.push_back(&widget);
            widget.window(window);
        }

        return id;
    }
    // for maybe new widgets; placement on another widget
    std::size_t placeWidget(const wci::PositionSpec& position, std::size_t parentId, wci::Widget& widget, wci::Anchor anchor)
    {
        if (widgetPlaced(widget) && parentId == getId(widget))
            throw std::invalid_argument("Widget cannot be its own parent");

        auto& parentPlaced = placements.at(parentId);
        auto* parentWidget = parentPlaced.widget;
        widget.position(position);
        widget.anchor(anchor);
        widget.absolute(calculateStackeeStart(position, parentPlaced.widget->size(), widget.size(), anchor));

        if (ids.contains(&widget))  // this widget has already been placed
        {
            if (widget.parentIsWindow())  // widget has been previously placed on window directly
            {
                std::erase(roots, &widget);
                parentPlaced.children.push_back(&widget);
                widget.parent(parentPlaced.widget);
            }
            else if (getId(widget.parent()) != parentId)  // widget has been placed on a different parent
            {
                auto& oldParentPlaced = placements.at(getId(widget.parent()));  // todo implement Widget::id()
                auto& newParentPlaced = placements.at(parentId);
                std::erase(oldParentPlaced.children, &widget);
                newParentPlaced.children.push_back(&widget);
                widget.parent(newParentPlaced.widget);
            }
            return ids.at(&widget);
        }

        // new widget
        auto id = counter++;
        placements[id] = WidgetPlacement{ &widget, std::vector<wci::Widget*>() };  // nb parentPlaced can get corrupted by a potential rehash of widgets
        ids[&widget] = id;
        placements.at(parentId).children.push_back(&widget);
        widget.parent(parentWidget);
        return id;
    }
    // for new widgets; placement on another widget
    std::size_t placeWidget(const wci::PositionSpec& position, std::size_t parentId, wci::Widget&& widget, wci::Anchor anchor)
    {
        orphans.push_back(std::move(widget));
        return placeWidget(position, parentId, orphans.back(), anchor);
    }
    // for already placed widgets; placement on another widget
    std::size_t placeWidget(const wci::PositionSpec& position, std::size_t parentId, std::size_t childId, wci::Anchor anchor)
    {
        if (parentId == childId)
            throw std::invalid_argument("Widget cannot be its own parent");

        auto& widget = getWidget(childId);
        widget.position(position);
        widget.anchor(anchor);

        if (widget.parentIsWindow())  // widget has been previously placed on window directly
        {
            auto& parentPlaced = placements.at(parentId);
            std::erase(roots, &widget);
            parentPlaced.children.push_back(&widget);
            widget.parent(parentPlaced.widget);
        }
        else if (getId(widget.parent()) != parentId)  // widget has been placed on a different parent
        {
            auto& oldParentPlaced = placements.at(getId(widget.parent()));  // todo implement Widget::id()
            auto& newParentPlaced = placements.at(parentId);
            std::erase(oldParentPlaced.children, &widget);
            newParentPlaced.children.push_back(&widget);
            widget.parent(newParentPlaced.widget);
        }

        widget.absolute(calculateStackeeStart(position, widget.parent().size(), widget.size(), anchor));  // now we are sure widget has right parent

        return childId;
    }

    void moveWidget(const wci::PositionSpec& position, wci::Widget& widget, wci::Anchor anchor)
    {
        if (!widget.placed())
            throw std::logic_error("Widget has not been yet placed");
        widget.position(position);
        widget.anchor(anchor);
        auto baseSize = widget.parentIsWindow() ? size() : widget.parent().size();
        widget.absolute(calculateStackeeStart(position, baseSize, widget.size(), anchor));
    }

    void removeIfOrphan(wci::Widget* orphan)
    {
        std::erase_if(orphans, [orphan](const wci::Widget& widget) -> bool { return &widget == orphan; });
    }

    void unplaceWidget(std::size_t id)
    {
        if (!widgetPlaced(id))
            throw std::invalid_argument("Such widget does not exist");

        auto& placed = placements.at(id);
        auto* widget = placed.widget;
        auto children = std::move(placed.children);
        placed.children.clear();  // restore assurance in the vector after the move

        if (widget->parentIsWindow())
            std::erase(roots, widget);
        else
        {
            auto& parentPlaced = placements.at(getId(widget->parent()));  // parentPlaced == placed inside unplaceWidget(*child)
            std::erase(parentPlaced.children, widget);
        }

        for (auto* child : children)
            unplaceWidget(*child);

        placements.erase(id);
        ids.erase(widget);
        widget->unplace();
        removeIfOrphan(widget);
    }
    void unplaceWidget(wci::Widget& widget)
    {
        if (!widgetPlaced(widget))
            throw std::invalid_argument("Such widget does not exist");
        unplaceWidget(getId(widget));
    }

    const std::vector<wci::Widget*>& getRoots() const
    {
        return roots;
    }

    const std::vector<wci::Widget*>& getChildren(std::size_t id) const
    {
        return placements.at(id).children;
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
    return impl->placeWidget(wci::PositionSpec{ x, y }, widget, anchor, this);
}
std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, wci::Widget&& widget, wci::Anchor anchor)
{
    return impl->placeWidget(wci::PositionSpec{ x, y }, std::move(widget), anchor, this);
}
std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, std::size_t id, wci::Anchor anchor)
{
    return impl->placeWidget(wci::PositionSpec{ x, y }, id, anchor, this);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, wci::Widget& widget, wci::Anchor anchor)
{
    return impl->placeWidget(position, widget, anchor, this);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, wci::Widget&& widget, wci::Anchor anchor)
{
    return impl->placeWidget(position, std::move(widget), anchor, this);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, std::size_t id, wci::Anchor anchor)
{
    return impl->placeWidget(position, id, anchor, this);
}

std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, std::size_t parentId, wci::Widget& child, wci::Anchor anchor)
{
    return impl->placeWidget(wci::PositionSpec{ x, y }, parentId, child, anchor);
}
std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, std::size_t parentId, wci::Widget&& child, wci::Anchor anchor)
{
    return impl->placeWidget(wci::PositionSpec{ x, y }, parentId, std::move(child), anchor);
}
std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, std::size_t parentId, std::size_t childId, wci::Anchor anchor)
{
    return impl->placeWidget(wci::PositionSpec{ x, y }, parentId, childId, anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, std::size_t parentId, wci::Widget& child, wci::Anchor anchor)
{
    return impl->placeWidget(position, parentId, child, anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, std::size_t parentId, wci::Widget&& child, wci::Anchor anchor)
{
    return impl->placeWidget(position, parentId, std::move(child), anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, std::size_t parentId, std::size_t childId, wci::Anchor anchor)
{
    return impl->placeWidget(position, parentId, childId, anchor);
}

std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, wci::Widget& parent, wci::Widget& child, wci::Anchor anchor)
{
    return impl->placeWidget(wci::PositionSpec{ x, y }, impl->getId(parent), child, anchor);
}
std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, wci::Widget& parent, wci::Widget&& child, wci::Anchor anchor)
{
    return impl->placeWidget(wci::PositionSpec{ x, y }, impl->getId(parent), std::move(child), anchor);
}
std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, wci::Widget& parent, std::size_t childId, wci::Anchor anchor)
{
    return impl->placeWidget(wci::PositionSpec{ x, y }, impl->getId(parent), childId, anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, wci::Widget& parent, wci::Widget& child, wci::Anchor anchor)
{
    return impl->placeWidget(position, impl->getId(parent), child, anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, wci::Widget& parent, wci::Widget&& child, wci::Anchor anchor)
{
    return impl->placeWidget(position, impl->getId(parent), std::move(child), anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, wci::Widget& parent, std::size_t childId, wci::Anchor anchor)
{
    return impl->placeWidget(position, impl->getId(parent), childId, anchor);
}

void wci::Window::move(const wci::LenPercent& x, const wci::LenPercent& y, wci::Widget& widget, wci::Anchor anchor)
{
    impl->moveWidget(wci::PositionSpec{ x, y }, widget, anchor);
}
void wci::Window::move(const wci::LenPercent& x, const wci::LenPercent& y, std::size_t id, wci::Anchor anchor)
{
    impl->moveWidget(wci::PositionSpec{ x, y }, impl->getWidget(id), anchor);
}
void wci::Window::move(const wci::PositionSpec& position, wci::Widget& widget, wci::Anchor anchor)
{
    impl->moveWidget(position, widget, anchor);
}
void wci::Window::move(const wci::PositionSpec& position, std::size_t id, wci::Anchor anchor)
{
    impl->moveWidget(position, impl->getWidget(id), anchor);
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
    return impl->getWidget(id);
}
const wci::Widget& wci::Window::widget(std::size_t id) const
{
    return impl->getWidget(id);
}

std::size_t wci::Window::id(const wci::Widget& widget) const
{
    return impl->getId(widget);
}

const std::vector<wci::Widget*>& wci::Window::children() const
{
    return impl->getRoots();
}
const std::vector<wci::Widget*>& wci::Window::children(const wci::Widget& widget) const
{
    return impl->getChildren(impl->getId(widget));
}
const std::vector<wci::Widget*>& wci::Window::children(std::size_t id) const
{
    return impl->getChildren(id);
}
