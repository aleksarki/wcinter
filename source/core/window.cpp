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
    struct {
        wci::Console console;
        wci::CharMatrix matrix;
    } inner;
    wci::Handle oldScreenBuffer;
    wci::CharMatrix framebuf;                                        // framebuf is used to forgo allocating matrices anew each rendering
    wci::CharMatrix lastbuf;                                         // contains last rendered frame
    std::list<wci::DisplayAdapter> orphans;                          // storage for rlavue-adapters
    wci::Hierarchy<wci::DisplayAdapter> hierarchy;                   // root node symbolises the window itself
    Id rootId;                                                       // id of the root (the window itself)
    bool forceRedraw;

    using Node = wci::Hierarchy<wci::DisplayAdapter>::Node<wci::DisplayAdapter>;

public:
    Impl() : inner{ wci::Console(), wci::CharMatrix(0, 0) }, framebuf(), lastbuf(), hierarchy(nullptr), forceRedraw(true)
    {
        inner.matrix.resize(inner.console.screenBufferInfo().size);
        oldScreenBuffer = inner.console.activeScreenBuffer();
        inner.console.activeScreenBuffer(wci::Console::createScreenBuffer());
        inner.console.cursorInfo(wci::CursorInfo{ 1, false });  // hide the cursor
        rootId = hierarchy.id(hierarchy.root());
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
    wci::CharMatrix renderNode(Node* node) /* const */
    {
        auto matrix = node->object->render();
        for (auto* child : node->children)
        {
            if (!child->object->visible())
                continue;
            auto* adapter = child->object;
            auto start = wci::calculateStackeeStart(adapter->position(), node->object->size(), adapter->size(), adapter->anchor());
            adapter->absolute(start);
            matrix.inlay(start, renderNode(child));
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

        for (auto* node : hierarchy.root()->children)
        {
            auto* adapter = node->object;
            if (!adapter->visible())
                continue;
            wci::Point start = calculateStackeeStart(adapter->position(), size(), adapter->size(), adapter->anchor());
            adapter->absolute(start);
            framebuf.inlay(start, renderNode(node));
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

private:
    wci::DisplayAdapter& orphanize(wci::DisplayAdapter&& adapter)
    {
        orphans.push_back(std::move(adapter));
        return orphans.back();
    }

public:
    wci::DisplayAdapter& getAdapter(wci::Id id)
    {
        if (!hierarchy.has(id))
            throw std::invalid_argument("Such adapter does not exist");
        return *hierarchy.node(id)->object;
    }

    bool adapterPlaced(wci::Id id) const noexcept
    {
        return hierarchy.has(id);
    }
    bool adapterPlaced(const wci::DisplayAdapter& adapter) const noexcept
    {
        return hierarchy.has(&adapter);
    }

    // for maybe new widgets; placement on window directly
    wci::Id placeAdapter(const wci::PositionSpec& position, wci::DisplayAdapter& adapter, wci::Anchor anchor, wci::Window* window)
    {
        adapter.position(position);
        adapter.anchor(anchor);
        adapter.absolute(calculateStackeeStart(position, size(), adapter.size(), anchor));
        auto id = hierarchy.id(&adapter);
        if (id == wci::npos)  // brand new adapter, yet to be placed
            id = hierarchy.add(&adapter, rootId);
        else
            hierarchy.move(id, rootId);
        adapter.windowp(window);
        adapter.id(id);
        return id;
    }
    // for new widgets; placement on window directly
    std::size_t placeAdapter(const wci::PositionSpec& position, wci::DisplayAdapter&& adapter, wci::Anchor anchor, wci::Window* window)
    {
        return placeAdapter(position, orphanize(std::move(adapter)), anchor, window);
    }
    // for already placed widgets; placement on window directly
    std::size_t placeAdapter(const wci::PositionSpec& position, wci::Id id, wci::Anchor anchor, wci::Window* window)
    {
        if (!hierarchy.has(id))
            throw std::invalid_argument("Such adapter does not exist");
        auto* adapter = hierarchy.node(id)->object;
        adapter->position(position);
        adapter->anchor(anchor);
        adapter->absolute(calculateStackeeStart(position, size(), adapter->size(), anchor));
        hierarchy.move(id, rootId);
        adapter->windowp(window);
        return id;
    }
    // for maybe new widgets; placement on another adapter
    std::size_t placeAdapter(const wci::PositionSpec& position, wci::Id parentId, wci::DisplayAdapter& child, wci::Anchor anchor)
    {
        if (!hierarchy.has(parentId))
            throw std::invalid_argument("Such parent adapter does not exist");
        auto* parent = hierarchy.node(parentId)->object;
        if (child.placed())
            for (auto* parentp = parent; parentp; parentp = parentp->parentp())  // do the check all the way up
                if (parentp->id() == child.id())
                    throw std::invalid_argument("DisplayAdapter cannot be its own parent");
        child.position(position);
        child.anchor(anchor);
        child.absolute(calculateStackeeStart(position, parent->size(), child.size(), anchor));
        auto id = hierarchy.id(&child);
        if (id == wci::npos)  // brand new adapter, yet to be placed
            id = hierarchy.add(&child, parentId);
        else
            hierarchy.move(id, parentId);
        child.parentp(parent);
        child.id(id);
        return id;
    }
    // for new widgets; placement on another adapter
    std::size_t placeAdapter(const wci::PositionSpec& position, std::size_t parentId, wci::DisplayAdapter&& adapter, wci::Anchor anchor)
    {
        return placeAdapter(position, parentId, orphanize(std::move(adapter)), anchor);
    }
    // for already placed widgets; placement on another adapter
    std::size_t placeAdapter(const wci::PositionSpec& position, std::size_t parentId, std::size_t childId, wci::Anchor anchor)
    {
        if (!hierarchy.has(childId))
            throw std::invalid_argument("Such child adapter does not exist");
        if (!hierarchy.has(parentId))
            throw std::invalid_argument("Such parent adapter does not exist");
        auto* child = hierarchy.node(childId)->object;
        auto* parent = hierarchy.node(parentId)->object;
        if (child->placed())
            for (auto* parentp = parent; parentp; parentp = parentp->parentp())  // do the check all the way up
                if (parentp->id() == childId)
                    throw std::invalid_argument("DisplayAdapter cannot be its own parent");
        child->position(position);
        child->anchor(anchor);
        child->absolute(calculateStackeeStart(position, parent->size(), child->size(), anchor));
        hierarchy.move(childId, parentId);
        child->parentp(parent);
        return childId;
    }

    // update placed adapter's positions
    void moveAdapter(const wci::PositionSpec& position, wci::DisplayAdapter& adapter, wci::Anchor anchor)
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

private:
    void markUnplaced(Node* node)
    {
        node->object->unplace();
        for (auto* child : node->children)
            markUnplaced(child);
        removeIfOrphan(node->object);
    }

public:
    void unplaceAdapter(wci::Id id)
    {
        if (!hierarchy.has(id))
            throw std::invalid_argument("Such adapter does not exist");
        auto* node = hierarchy.node(id);
        markUnplaced(node);
        hierarchy.remove(id);
    }
    void unplaceAdapter(wci::DisplayAdapter& adapter)
    {
        if (!adapter.placed())
            throw std::invalid_argument("Such adapter does not exist");
        unplaceAdapter(adapter.id());
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
    impl->moveAdapter(wci::position(x, y), adapter, anchor);
}
void wci::Window::move(const wci::LenPercent& x, const wci::LenPercent& y, std::size_t id, wci::Anchor anchor)
{
    impl->moveAdapter(wci::position(x, y), impl->getAdapter(id), anchor);
}
void wci::Window::move(const wci::PositionSpec& position, wci::DisplayAdapter& adapter, wci::Anchor anchor)
{
    impl->moveAdapter(position, adapter, anchor);
}
void wci::Window::move(const wci::PositionSpec& position, std::size_t id, wci::Anchor anchor)
{
    impl->moveAdapter(position, impl->getAdapter(id), anchor);
}

void wci::Window::unplace(std::size_t id)
{
    impl->unplaceAdapter(id);
}
void wci::Window::unplace(wci::DisplayAdapter& adapter)
{
    impl->unplaceAdapter(adapter);
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

// const std::vector<wci::DisplayAdapter*>& wci::Window::children() const
// {
//     return impl->getRoots();
// }
// const std::vector<wci::DisplayAdapter*>& wci::Window::children(const wci::DisplayAdapter& adapter) const
// {
//     return impl->getChildren(adapter.id());
// }
// const std::vector<wci::DisplayAdapter*>& wci::Window::children(std::size_t id) const
// {
//     return impl->getChildren(id);
// }
