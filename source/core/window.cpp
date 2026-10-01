#define UNICODE
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <algorithm>
#include <cmath>
#include <list>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>
#include <windows.h>

#include "window.hpp"
#include "../util.hpp"
#include "../util/apicast.hpp"

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
    wci::CharMatrix renderPlacedWidget(const WidgetPlacement& placed) const
    {
        auto matrix = placed.widget->render();
        for (auto* child : placed.children)
        {
            if (!child->visible())
                continue;
            auto& childPlaced = placements.at(child->id());
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
            auto& placed = placements.at(root->id());
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
        return widget.placed() && placements.contains(widget.id());
    }

    wci::Widget& getWidget(std::size_t id)
    {
        return *placements.at(id).widget;
    }
    const wci::Widget& getWidget(std::size_t id) const
    {
        return *placements.at(id).widget;
    }

private:
    std::size_t newPlacement(wci::Widget& widget)
    {
        auto id = counter++;
        placements[id] = WidgetPlacement{ &widget, std::vector<wci::Widget*>() };
        return id;
    }

    wci::Widget& newOrphan(wci::Widget&& widget)
    {
        orphans.push_back(std::move(widget));
        return orphans.back();
    }

    void addRoot(wci::Widget& widget)
    {
        roots.push_back(&widget);
    }

    void removeRoot(const wci::Widget& widget)
    {
        std::erase(roots, &widget);
    }

    void addChild(wci::Widget& child, wci::Widget& parent)
    {
        placements.at(parent.id()).children.push_back(&child);
    }

    void removeChild(const wci::Widget& child, const wci::Widget& parent)
    {
        std::erase(placements.at(parent.id()).children, &child);
    }

public:
    // for maybe new widgets; placement on window directly
    std::size_t placeWidget(const wci::PositionSpec& position, wci::Widget& widget, wci::Anchor anchor, wci::Window* window)
    {
        widget.position(position);
        widget.anchor(anchor);
        widget.absolute(calculateStackeeStart(position, size(), widget.size(), anchor));

        if (widget.parentIsWindow())  // already placed on the window
            return widget.id();
        else if (widget.parentIsWidget())  // widget has been previously placed on another widget
            removeChild(widget, widget.parent());
        else  // new widget
            widget.id(newPlacement(widget));
        addRoot(widget);
        widget.windowp(window);

        return widget.id();
    }
    // for new widgets; placement on window directly
    std::size_t placeWidget(const wci::PositionSpec& position, wci::Widget&& widget, wci::Anchor anchor, wci::Window* window)
    {
        return placeWidget(position, newOrphan(std::move(widget)), anchor, window);
    }
    // for already placed widgets; placement on window directly
    std::size_t placeWidget(const wci::PositionSpec& position, std::size_t id, wci::Anchor anchor, wci::Window* window)
    {
        if (!widgetPlaced(id))
            throw std::invalid_argument("Such widget does not exist");
    
        auto& widget = getWidget(id);
        widget.position(position);
        widget.anchor(anchor);
        widget.absolute(calculateStackeeStart(position, size(), widget.size(), anchor));
    
        if (widget.parentIsWidget())  // widget has been previously placed on another widget
        {
            removeChild(widget, widget.parent());
            addRoot(widget);
            widget.windowp(window);
        }

        return id;
    }
    // for maybe new widgets; placement on another widget
    std::size_t placeWidget(const wci::PositionSpec& position, std::size_t parentId, wci::Widget& child, wci::Anchor anchor)
    {
        if (!widgetPlaced(parentId))
            throw std::invalid_argument("Such parent widget does not exist");

        auto& parent = getWidget(parentId);
        if (child.placed())
            for (auto* parentp = &parent; parentp; parentp = parentp->parentp())  // do the check all the way up
                if (parentp->id() == child.id())
                    throw std::invalid_argument("Widget cannot be its own parent");

        child.position(position);
        child.anchor(anchor);
        child.absolute(calculateStackeeStart(position, parent.size(), child.size(), anchor));

        if (child.parentIsWindow())  // widget has been previously placed on window directly
            removeRoot(child);
        else if (child.parentIsWidget())
        {
            if (child.parent().id() == parentId)  // widget's already on the right parent
                return child.id();
            removeChild(child, child.parent());  // widget has been placed on a different parent
        }
        else  // new widget
            child.id(newPlacement(child));
        addChild(child, parent);
        child.parentp(&parent);

        return child.id();
    }
    // for new widgets; placement on another widget
    std::size_t placeWidget(const wci::PositionSpec& position, std::size_t parentId, wci::Widget&& widget, wci::Anchor anchor)
    {
        return placeWidget(position, parentId, newOrphan(std::move(widget)), anchor);
    }
    // for already placed widgets; placement on another widget
    std::size_t placeWidget(const wci::PositionSpec& position, std::size_t parentId, std::size_t childId, wci::Anchor anchor)  // review what happens when circular dependecy?
    {
        if (!widgetPlaced(childId))
            throw std::invalid_argument("Such child widget does not exist");
        if (!widgetPlaced(parentId))
            throw std::invalid_argument("Such parent widget does not exist");

        auto& child = getWidget(childId);
        auto& parent = getWidget(parentId);
        if (child.placed())
            for (auto* parentp = &parent; parentp; parentp = parentp->parentp())  // do the check all the way up
                if (parentp->id() == childId)
                    throw std::invalid_argument("Widget cannot be its own parent");

        child.position(position);
        child.anchor(anchor);
        child.absolute(calculateStackeeStart(position, parent.size(), child.size(), anchor));

        if (child.parentIsWindow())  // widget has been previously placed on window directly
            removeRoot(child);
        else if (child.parentIsWidget())
        {
            if (child.parent().id() == parentId)  // widget's already on the right parent
                return childId;
            removeChild(child, child.parent());  // widget has been placed on a different parent
        }
        addChild(child, parent);
        child.parentp(&parent);

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
            auto& parentPlaced = placements.at(widget->parent().id());  // parentPlaced == placed inside unplaceWidget(*child)
            std::erase(parentPlaced.children, widget);
        }

        for (auto* child : children)
            unplaceWidget(*child);

        placements.erase(id);
        widget->unplace();
        removeIfOrphan(widget);
    }
    void unplaceWidget(wci::Widget& widget)
    {
        if (!widget.placed())
            throw std::invalid_argument("Such widget does not exist");
        unplaceWidget(widget.id());
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
    return impl->placeWidget(wci::PositionSpec{ x, y }, parent.id(), child, anchor);
}
std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, wci::Widget& parent, wci::Widget&& child, wci::Anchor anchor)
{
    return impl->placeWidget(wci::PositionSpec{ x, y }, parent.id(), std::move(child), anchor);
}
std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, wci::Widget& parent, std::size_t childId, wci::Anchor anchor)
{
    return impl->placeWidget(wci::PositionSpec{ x, y }, parent.id(), childId, anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, wci::Widget& parent, wci::Widget& child, wci::Anchor anchor)
{
    return impl->placeWidget(position, parent.id(), child, anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, wci::Widget& parent, wci::Widget&& child, wci::Anchor anchor)
{
    return impl->placeWidget(position, parent.id(), std::move(child), anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, wci::Widget& parent, std::size_t childId, wci::Anchor anchor)
{
    return impl->placeWidget(position, parent.id(), childId, anchor);
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
    return widget.id();
}

const std::vector<wci::Widget*>& wci::Window::children() const
{
    return impl->getRoots();
}
const std::vector<wci::Widget*>& wci::Window::children(const wci::Widget& widget) const
{
    return impl->getChildren(widget.id());
}
const std::vector<wci::Widget*>& wci::Window::children(std::size_t id) const
{
    return impl->getChildren(id);
}
