#define UNICODE
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <algorithm>
#include <cstddef>
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
    struct AdapterPlacement
    {
        wci::DisplayAdapter* adapter;
        std::vector<wci::DisplayAdapter*> children;
    };

    struct {
        wci::Console console;
        wci::CharMatrix matrix;
    } inner;
    wci::Handle oldScreenBuffer;
    wci::CharMatrix framebuf;                                      // framebuf is used to forgo allocating matrices anew each rendering
    wci::CharMatrix lastbuf;                                       // contains last rendered frame
    std::unordered_map<std::size_t, AdapterPlacement> placements;  // id -> adapter placement info
    std::vector<wci::DisplayAdapter*> roots;                       // adapters placed directly on the window
    std::list<wci::DisplayAdapter> orphans;                        // list because it does not invalidate pointers on relocation and deletion
    std::size_t counter = 0;
    bool forceRedraw;

public:
    Impl() : inner{ wci::Console(), wci::CharMatrix(0, 0) }, framebuf(), lastbuf(), forceRedraw(true)
    {
        inner.matrix.resize(inner.console.screenBufferInfo().size);
        oldScreenBuffer = inner.console.activeScreenBuffer();
        inner.console.activeScreenBuffer(wci::Console::createScreenBuffer());
        inner.console.cursorInfo(wci::CursorInfo{ 1, false });  // hide the cursor
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

    wci::Geometry size() const
    {
        return matrix().size();
    }

    void refit()
    {
        auto newSize = console().screenBufferInfo().size;
        if (newSize == size())
            return;
        wci::CharMatrix newMatrix(newSize);
        newMatrix.inlay(matrix());
        matrix().swap(newMatrix);
    }

private:
    wci::CharMatrix renderPlacedAdapter(const AdapterPlacement& placed) const
    {
        auto matrix = placed.adapter->render();
        for (auto* child : placed.children)
        {
            if (!child->visible())
                continue;
            auto& childPlaced = placements.at(child->id());
            wci::Point startPoint = calculateStackeeStart(
                child->position(), placed.adapter->size(), child->size(), child->anchor()
            );
            child->absolute(startPoint);
            matrix.inlay(startPoint, renderPlacedAdapter(childPlaced));
        }
        return matrix;
    }

public:
    void render()
    {
        refit();
        if (matrix().empty())
            return;

        if (framebuf.size() != size())  // resize happened
        {
            framebuf.resize(size());
            lastbuf.resize(size());
            forceRedraw = true;
        }
        std::copy(matrix().data(), matrix().data() + size().area(), framebuf.data());

        for (auto* root : roots)
        {
            auto& placed = placements.at(root->id());
            auto* adapter = placed.adapter;
            if (!adapter->visible())
                continue;
            wci::Point startPoint = calculateStackeeStart(
                adapter->position(), size(), adapter->size(), adapter->anchor()
            );
            adapter->absolute(startPoint);
            framebuf.inlay(startPoint, renderPlacedAdapter(placed));
        }

        wci::Rect bbox;
        if (forceRedraw)
        {
            bbox = wci::Rect{ 0, 0, size().width, size().height };
            forceRedraw = false;
        }
        else
        {
            auto mask = framebuf.difference(lastbuf);
            bbox = mask.boundingBox();
            if (bbox == wci::norect)
                return;        
        }

        for (wci::Short i = bbox.top; i < bbox.bottom; ++i)
        {
            const auto* source = framebuf.rowData(i) + bbox.left;
            auto* dest = lastbuf.rowData(i) + bbox.left;
            // todo implement CharMatrix:: copy(from, to, other), inlay(mask, other)
            std::copy(source, source + bbox.size().width, dest);
        }

        SMALL_RECT rect = wci::api(bbox);
        --rect.Right;  // SMALL_RECT is inclusive
        --rect.Bottom;
        WriteConsoleOutputW(
            wci::api(console().stdOutput()),  // same as console().activeScreenBuffer()
            wci::api(framebuf.data()),
            wci::api(framebuf.size()),
            wci::api(bbox.topLeft()),
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
    void putString(wci::Short x, wci::Short y, const wci::CharCell cells[], wci::Short length)
    {
        for (wci::Short i = 0; i < length; ++i)
            matrix().put(x + i, y, cells[i]);
    }

    void putMatrix(wci::Short x, wci::Short y, const wci::CharMatrix& matrix)
    {
        this->matrix().inlay(x, y, matrix);
    }

    bool adapterPlaced(std::size_t id) const noexcept
    {
        return placements.contains(id);
    }
    bool adapterPlaced(const wci::DisplayAdapter& adapter) const noexcept
    {
        return adapter.placed() && placements.contains(adapter.id());
    }

    wci::DisplayAdapter& getAdapter(std::size_t id)
    {
        return *placements.at(id).adapter;
    }
    const wci::DisplayAdapter& getAdapter(std::size_t id) const
    {
        return *placements.at(id).adapter;
    }

private:
    std::size_t newPlacement(wci::DisplayAdapter& adapter)
    {
        auto id = counter++;
        placements[id] = AdapterPlacement{ &adapter, std::vector<wci::DisplayAdapter*>() };
        return id;
    }

    wci::DisplayAdapter& newOrphan(wci::DisplayAdapter&& adapter)
    {
        orphans.push_back(std::move(adapter));
        return orphans.back();
    }

    void addRoot(wci::DisplayAdapter& adapter)
    {
        roots.push_back(&adapter);
    }

    void removeRoot(const wci::DisplayAdapter& adapter)
    {
        std::erase(roots, &adapter);
    }

    void addChild(wci::DisplayAdapter& child, wci::DisplayAdapter& parent)
    {
        placements.at(parent.id()).children.push_back(&child);
    }

    void removeChild(const wci::DisplayAdapter& child, const wci::DisplayAdapter& parent)
    {
        std::erase(placements.at(parent.id()).children, &child);
    }

public:
    // for maybe new widgets; placement on window directly
    std::size_t placeAdapter(const wci::PositionSpec& position, wci::DisplayAdapter& adapter, wci::Anchor anchor, wci::Window* window)
    {
        adapter.position(position);
        adapter.anchor(anchor);
        adapter.absolute(calculateStackeeStart(position, size(), adapter.size(), anchor));

        if (adapter.parentIsWindow())  // already placed on the window
            return adapter.id();
        else if (adapter.parentIsAdapter())  // adapter has been previously placed on another adapter
            removeChild(adapter, adapter.parent());
        else  // new adapter
            adapter.id(newPlacement(adapter));
        addRoot(adapter);
        adapter.windowp(window);

        return adapter.id();
    }
    // for new widgets; placement on window directly
    std::size_t placeAdapter(const wci::PositionSpec& position, wci::DisplayAdapter&& adapter, wci::Anchor anchor, wci::Window* window)
    {
        return placeAdapter(position, newOrphan(std::move(adapter)), anchor, window);
    }
    // for already placed widgets; placement on window directly
    std::size_t placeAdapter(const wci::PositionSpec& position, std::size_t id, wci::Anchor anchor, wci::Window* window)
    {
        if (!adapterPlaced(id))
            throw std::invalid_argument("Such adapter does not exist");
    
        auto& adapter = getAdapter(id);
        adapter.position(position);
        adapter.anchor(anchor);
        adapter.absolute(calculateStackeeStart(position, size(), adapter.size(), anchor));
    
        if (adapter.parentIsAdapter())  // adapter has been previously placed on another adapter
        {
            removeChild(adapter, adapter.parent());
            addRoot(adapter);
            adapter.windowp(window);
        }

        return id;
    }
    // for maybe new widgets; placement on another adapter
    std::size_t placeAdapter(const wci::PositionSpec& position, std::size_t parentId, wci::DisplayAdapter& child, wci::Anchor anchor)
    {
        if (!adapterPlaced(parentId))
            throw std::invalid_argument("Such parent adapter does not exist");

        auto& parent = getAdapter(parentId);
        if (child.placed())
            for (auto* parentp = &parent; parentp; parentp = parentp->parentp())  // do the check all the way up
                if (parentp->id() == child.id())
                    throw std::invalid_argument("DisplayAdapter cannot be its own parent");

        child.position(position);
        child.anchor(anchor);
        child.absolute(calculateStackeeStart(position, parent.size(), child.size(), anchor));

        if (child.parentIsWindow())  // adapter has been previously placed on window directly
            removeRoot(child);
        else if (child.parentIsAdapter())
        {
            if (child.parent().id() == parentId)  // adapter's already on the right parent
                return child.id();
            removeChild(child, child.parent());  // adapter has been placed on a different parent
        }
        else  // new adapter
            child.id(newPlacement(child));
        addChild(child, parent);
        child.parentp(&parent);

        return child.id();
    }
    // for new widgets; placement on another adapter
    std::size_t placeAdapter(const wci::PositionSpec& position, std::size_t parentId, wci::DisplayAdapter&& adapter, wci::Anchor anchor)
    {
        return placeAdapter(position, parentId, newOrphan(std::move(adapter)), anchor);
    }
    // for already placed widgets; placement on another adapter
    std::size_t placeAdapter(const wci::PositionSpec& position, std::size_t parentId, std::size_t childId, wci::Anchor anchor)  // review what happens when circular dependecy?
    {
        if (!adapterPlaced(childId))
            throw std::invalid_argument("Such child adapter does not exist");
        if (!adapterPlaced(parentId))
            throw std::invalid_argument("Such parent adapter does not exist");

        auto& child = getAdapter(childId);
        auto& parent = getAdapter(parentId);
        if (child.placed())
            for (auto* parentp = &parent; parentp; parentp = parentp->parentp())  // do the check all the way up
                if (parentp->id() == childId)
                    throw std::invalid_argument("DisplayAdapter cannot be its own parent");

        child.position(position);
        child.anchor(anchor);
        child.absolute(calculateStackeeStart(position, parent.size(), child.size(), anchor));

        if (child.parentIsWindow())  // adapter has been previously placed on window directly
            removeRoot(child);
        else if (child.parentIsAdapter())
        {
            if (child.parent().id() == parentId)  // adapter's already on the right parent
                return childId;
            removeChild(child, child.parent());  // adapter has been placed on a different parent
        }
        addChild(child, parent);
        child.parentp(&parent);

        return childId;
    }

    void moveWidget(const wci::PositionSpec& position, wci::DisplayAdapter& adapter, wci::Anchor anchor)
    {
        if (!adapter.placed())
            throw std::logic_error("DisplayAdapter has not been yet placed");
        adapter.position(position);
        adapter.anchor(anchor);
        auto baseSize = adapter.parentIsWindow() ? size() : adapter.parent().size();
        adapter.absolute(calculateStackeeStart(position, baseSize, adapter.size(), anchor));
    }

    void removeIfOrphan(wci::DisplayAdapter* orphan)
    {
        std::erase_if(orphans, [orphan](const wci::DisplayAdapter& adapter) -> bool { return &adapter == orphan; });
    }

    void unplaceWidget(std::size_t id)
    {
        if (!adapterPlaced(id))
            throw std::invalid_argument("Such adapter does not exist");

        auto& placed = placements.at(id);
        auto* adapter = placed.adapter;
        auto children = std::move(placed.children);
        placed.children.clear();  // restore assurance in the vector after the move

        if (adapter->parentIsWindow())
            std::erase(roots, adapter);
        else
        {
            auto& parentPlaced = placements.at(adapter->parent().id());  // parentPlaced == placed inside unplaceWidget(*child)
            std::erase(parentPlaced.children, adapter);
        }

        for (auto* child : children)
            unplaceWidget(*child);

        placements.erase(id);
        adapter->unplace();
        removeIfOrphan(adapter);
    }
    void unplaceWidget(wci::DisplayAdapter& adapter)
    {
        if (!adapter.placed())
            throw std::invalid_argument("Such adapter does not exist");
        unplaceWidget(adapter.id());
    }

    const std::vector<wci::DisplayAdapter*>& getRoots() const
    {
        return roots;
    }

    const std::vector<wci::DisplayAdapter*>& getChildren(std::size_t id) const
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

wci::Geometry wci::Window::size() const
{
    return impl->size();
}

void wci::Window::refit()
{
    impl->refit();
}

void wci::Window::render()
{
    impl->render();
}

void wci::Window::fill(wci::Wchar character, wci::Attribute attributes)
{
    impl->matrix().fill(character, attributes);
}
void wci::Window::fill(const wci::CharCell& cell)
{
    impl->matrix().fill(cell);
}

void wci::Window::blank()
{
    impl->matrix().blank();
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
void wci::Window::put(wci::Short x, wci::Short y, const wci::CharCell& cell)
{
    impl->matrix().put(x, y, cell);
}
void wci::Window::put(const wci::Point& position, const wci::CharCell& cell)
{
    impl->matrix().put(position, cell);
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
void wci::Window::put(wci::Short x, wci::Short y, const std::wstring& string)
{
    auto attributes = impl->console().screenBufferInfo().attributes;
    impl->putString(x, y, string.data(), attributes);
}
void wci::Window::put(const wci::Point& position, const wci::CharCell cells[], wci::Short length)
{
    impl->putString(position.x, position.y, cells, length);
}
void wci::Window::put(const wci::Point& position, const std::wstring& string)
{
    auto attributes = impl->console().screenBufferInfo().attributes;
    impl->putString(position.x, position.y, string.data(), attributes);
}
void wci::Window::put(wci::Short x, wci::Short y, const wci::CharMatrix& matrix)
{
    impl->putMatrix(x, y, matrix);
}
void wci::Window::put(const wci::Point& position, const wci::CharMatrix& matrix)
{
    impl->putMatrix(position.x, position.y, matrix);
}

std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, wci::DisplayAdapter& adapter, wci::Anchor anchor)
{
    return impl->placeAdapter(wci::position(x, y), adapter, anchor, this);
}
std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, wci::DisplayAdapter&& adapter, wci::Anchor anchor)
{
    return impl->placeAdapter(wci::position(x, y), std::move(adapter), anchor, this);
}
std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, std::size_t id, wci::Anchor anchor)
{
    return impl->placeAdapter(wci::position(x, y), id, anchor, this);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, wci::DisplayAdapter& adapter, wci::Anchor anchor)
{
    return impl->placeAdapter(position, adapter, anchor, this);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, wci::DisplayAdapter&& adapter, wci::Anchor anchor)
{
    return impl->placeAdapter(position, std::move(adapter), anchor, this);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, std::size_t id, wci::Anchor anchor)
{
    return impl->placeAdapter(position, id, anchor, this);
}

std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, std::size_t parentId, wci::DisplayAdapter& child, wci::Anchor anchor)
{
    return impl->placeAdapter(wci::position(x, y), parentId, child, anchor);
}
std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, std::size_t parentId, wci::DisplayAdapter&& child, wci::Anchor anchor)
{
    return impl->placeAdapter(wci::position(x, y), parentId, std::move(child), anchor);
}
std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, std::size_t parentId, std::size_t childId, wci::Anchor anchor)
{
    return impl->placeAdapter(wci::position(x, y), parentId, childId, anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, std::size_t parentId, wci::DisplayAdapter& child, wci::Anchor anchor)
{
    return impl->placeAdapter(position, parentId, child, anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, std::size_t parentId, wci::DisplayAdapter&& child, wci::Anchor anchor)
{
    return impl->placeAdapter(position, parentId, std::move(child), anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, std::size_t parentId, std::size_t childId, wci::Anchor anchor)
{
    return impl->placeAdapter(position, parentId, childId, anchor);
}

std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, wci::DisplayAdapter& parent, wci::DisplayAdapter& child, wci::Anchor anchor)
{
    return impl->placeAdapter(wci::position(x, y), parent.id(), child, anchor);
}
std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, wci::DisplayAdapter& parent, wci::DisplayAdapter&& child, wci::Anchor anchor)
{
    return impl->placeAdapter(wci::position(x, y), parent.id(), std::move(child), anchor);
}
std::size_t wci::Window::place(const wci::LenPercent& x, const wci::LenPercent& y, wci::DisplayAdapter& parent, std::size_t childId, wci::Anchor anchor)
{
    return impl->placeAdapter(wci::position(x, y), parent.id(), childId, anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, wci::DisplayAdapter& parent, wci::DisplayAdapter& child, wci::Anchor anchor)
{
    return impl->placeAdapter(position, parent.id(), child, anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, wci::DisplayAdapter& parent, wci::DisplayAdapter&& child, wci::Anchor anchor)
{
    return impl->placeAdapter(position, parent.id(), std::move(child), anchor);
}
std::size_t wci::Window::place(const wci::PositionSpec& position, wci::DisplayAdapter& parent, std::size_t childId, wci::Anchor anchor)
{
    return impl->placeAdapter(position, parent.id(), childId, anchor);
}

void wci::Window::move(const wci::LenPercent& x, const wci::LenPercent& y, wci::DisplayAdapter& adapter, wci::Anchor anchor)
{
    impl->moveWidget(wci::position(x, y), adapter, anchor);
}
void wci::Window::move(const wci::LenPercent& x, const wci::LenPercent& y, std::size_t id, wci::Anchor anchor)
{
    impl->moveWidget(wci::position(x, y), impl->getAdapter(id), anchor);
}
void wci::Window::move(const wci::PositionSpec& position, wci::DisplayAdapter& adapter, wci::Anchor anchor)
{
    impl->moveWidget(position, adapter, anchor);
}
void wci::Window::move(const wci::PositionSpec& position, std::size_t id, wci::Anchor anchor)
{
    impl->moveWidget(position, impl->getAdapter(id), anchor);
}

void wci::Window::unplace(std::size_t id)
{
    impl->unplaceWidget(id);
}
void wci::Window::unplace(wci::DisplayAdapter& adapter)
{
    impl->unplaceWidget(adapter);
}

bool wci::Window::placed(std::size_t id) const noexcept
{
    return impl->adapterPlaced(id);
}
bool wci::Window::placed(const DisplayAdapter& adapter) const noexcept
{
    return impl->adapterPlaced(adapter);
}

wci::DisplayAdapter& wci::Window::adapter(std::size_t id)
{
    return impl->getAdapter(id);
}
const wci::DisplayAdapter& wci::Window::adapter(std::size_t id) const
{
    return impl->getAdapter(id);
}

std::size_t wci::Window::id(const wci::DisplayAdapter& adapter) const
{
    return adapter.id();
}

const std::vector<wci::DisplayAdapter*>& wci::Window::children() const
{
    return impl->getRoots();
}
const std::vector<wci::DisplayAdapter*>& wci::Window::children(const wci::DisplayAdapter& adapter) const
{
    return impl->getChildren(adapter.id());
}
const std::vector<wci::DisplayAdapter*>& wci::Window::children(std::size_t id) const
{
    return impl->getChildren(id);
}
