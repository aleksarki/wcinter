#define UNICODE
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <algorithm>
#include <list>
#include <stdexcept>
#include <unordered_map>
#include <utility>
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
    wci::CharMatrix framebuf;                                       // framebuf is used to forgo allocating matrices anew each rendering
    wci::CharMatrix lastbuf;                                        // contains last rendered frame
    std::list<wci::DisplayAdapter> orphans;                         // storage for rlavue-adapters
    std::list<wci::Placement<wci::DisplayAdapter>> placements;      // storage for placements
    wci::Hierarchy<wci::Placement<wci::DisplayAdapter>> hierarchy;  // node -> placement -> adapter; root node symbolises the window itself
    std::unordered_map<const wci::DisplayAdapter*, wci::Id> ids;    // adapter -> id
    wci::Id rootId;                                                 // id of the root (the window itself)
    bool forceRedraw;

    using Node = wci::Hierarchy<wci::Placement<wci::DisplayAdapter>>::Node<wci::Placement<wci::DisplayAdapter>>;

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
        // node -> placement -> adapter
        auto matrix = node->object->object->render();
        for (auto* child : node->children)
        {
            if (!child->object->object->visible())
                continue;
            auto start = child->object->absolute(node->object->object->size());
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

        // node -> placement -> adapter
        for (auto* node : hierarchy.root()->children)
        {
            if (!node->object->object->visible())
                continue;
            auto start = node->object->absolute(size());
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
        return *hierarchy.node(id)->object->object;
    }

    bool adapterPlaced(wci::Id id) const noexcept
    {
        return hierarchy.has(id);
    }
    bool adapterPlaced(const wci::DisplayAdapter& adapter) const noexcept
    {
        return ids.contains(&adapter);
    }

    wci::Id getId(const wci::DisplayAdapter& adapter) const noexcept
    {
        if (!adapterPlaced(adapter))
            return wci::npos;
        return ids.at(&adapter);
    }

private:
    wci::Placement<wci::DisplayAdapter>* createPlacement(
        wci::DisplayAdapter& adapter, const wci::PositionSpec& position, wci::Anchor anchor, int z
    )
    {
        placements.emplace_back(&adapter, position, anchor, z);
        return &placements.back();
    }

public:
    // for maybe new widgets; placement on window directly
    wci::Id placeAdapter(wci::DisplayAdapter& adapter, const wci::PositionSpec& position, wci::Anchor anchor, int z)
    {
        // fixme z order
        wci::Id id = getId(adapter);
        if (id == wci::npos)  // brand new adapter, yet to be placed
        {
            id = hierarchy.add(createPlacement(adapter, position, anchor, z), rootId);
            ids[&adapter] = id;
        }
        else
        {
            hierarchy.move(id, rootId);
            moveAdapter(id, position, anchor, z);
        }
        return id;
    }
    // for new widgets; placement on window directly
    wci::Id placeAdapter(wci::DisplayAdapter&& adapter, const wci::PositionSpec& position, wci::Anchor anchor, int z)
    {
        return placeAdapter(orphanize(std::move(adapter)), position, anchor, z);
    }
    // for already placed widgets; placement on window directly
    wci::Id placeAdapter(wci::Id id, const wci::PositionSpec& position, wci::Anchor anchor, int z)
    {
        if (!hierarchy.has(id))
            throw std::invalid_argument("Such adapter does not exist");
        hierarchy.move(id, rootId);
        moveAdapter(id, position, anchor, z);
        return id;
    }
    // for maybe new widgets; placement on another adapter
    wci::Id placeAdapter(wci::DisplayAdapter& child, wci::Id parentId, const wci::PositionSpec& position, wci::Anchor anchor, int z)
    {
        // fixme z order
        if (!hierarchy.has(parentId))
            throw std::invalid_argument("Such parent adapter does not exist");
        if (adapterPlaced(child))
            for (auto* parentNode = hierarchy.node(parentId); parentNode; parentNode = parentNode->parent)  // do the check all the way up
                if (parentNode->object && parentNode->object->object == &child)
                    throw std::invalid_argument("DisplayAdapter cannot be its own parent");
        auto childId = getId(child);
        if (childId == wci::npos)  // brand new adapter, yet to be placed
        {
            childId = hierarchy.add(createPlacement(child, position, anchor, z), parentId);
            ids[&child] = childId;
        }
        else
        {
            hierarchy.move(childId, parentId);
            moveAdapter(childId, position, anchor, z);
        }
        return childId;
    }
    // for new widgets; placement on another adapter
    wci::Id placeAdapter(wci::DisplayAdapter&& child, wci::Id parentId, const wci::PositionSpec& position, wci::Anchor anchor, int z)
    {
        return placeAdapter(orphanize(std::move(child)), parentId, position, anchor, z);
    }
    // for already placed widgets; placement on another adapter
    wci::Id placeAdapter(wci::Id childId, wci::Id parentId, const wci::PositionSpec& position, wci::Anchor anchor, int z)
    {
        if (!hierarchy.has(childId))
            throw std::invalid_argument("Such child adapter does not exist");
        if (!hierarchy.has(parentId))
            throw std::invalid_argument("Such parent adapter does not exist");
        auto* child = hierarchy.node(childId)->object->object;
        for (auto* parentNode = hierarchy.node(parentId); parentNode; parentNode = parentNode->parent)  // do the check all the way up
                if (parentNode->object && parentNode->object->object == child)
                    throw std::invalid_argument("DisplayAdapter cannot be its own parent");
        hierarchy.move(childId, parentId);
        moveAdapter(childId, position, anchor, z);
        return childId;
    }

    // update placed adapter's positions
    void moveAdapter(wci::Id id, const wci::PositionSpec& position, wci::Anchor anchor, int z)
    {
        if (!adapterPlaced(id))
            throw std::logic_error("DisplayAdapter has not been yet placed");
        auto* placement = hierarchy.node(id)->object;
        placement->position = position;
        placement->anchor = anchor;
        placement->z = z;  // fixme z order
    }

    void repositionAdapter(wci::Id id, const wci::PositionSpec& position)
    {
        if (!adapterPlaced(id))
            throw std::logic_error("DisplayAdapter has not been yet placed");
        auto* placement = hierarchy.node(id)->object;
        placement->position = position;
    }

    void reanchorAdapter(wci::Id id, wci::Anchor anchor)
    {
        if (!adapterPlaced(id))
            throw std::logic_error("DisplayAdapter has not been yet placed");
        auto* placement = hierarchy.node(id)->object;
        placement->anchor = anchor;
    }

    void reorderAdapter(wci::Id id, int z)
    {
        if (!adapterPlaced(id))
            throw std::logic_error("DisplayAdapter has not been yet placed");
        auto* placement = hierarchy.node(id)->object;
        placement->z = z;  // fixme z order
    }

    wci::Point getAbsolute(wci::Id id) const
    {
        auto* node = hierarchy.node(id);
        if (node == hierarchy.nullnode)
            throw std::logic_error("DisplayAdapter has not been yet placed");
        return node->object->absolute(size());
    }

private:
    void markUnplaced(Node* node)
    {
        for (auto* child : node->children)
            markUnplaced(child);
        ids.erase(node->object->object);
        std::erase_if(orphans, [orphan = node->object->object](const wci::DisplayAdapter& adapter) -> bool  // remove if orphan
        {
            return &adapter == orphan;
        });
        std::erase_if(placements, [placement = node->object](const auto& p) -> bool
        {
            return &p == placement;
        });
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
    void unplaceAdapter(const wci::DisplayAdapter& adapter)
    {
        unplaceAdapter(getId(adapter));
    }
};

wci::Window::Window() : impl(std::make_unique<Impl>())
{}

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

wci::Id wci::Window::place(wci::DisplayAdapter& adapter, const wci::LenPercent& x, const wci::LenPercent& y, wci::Anchor anchor, int z)
{
    return impl->placeAdapter(adapter, wci::position(x, y), anchor, z);
}
wci::Id wci::Window::place(wci::DisplayAdapter&& adapter, const wci::LenPercent& x, const wci::LenPercent& y, wci::Anchor anchor, int z)
{
    return impl->placeAdapter(std::move(adapter), wci::position(x, y), anchor, z);
}
wci::Id wci::Window::place(wci::Id id, const wci::LenPercent& x, const wci::LenPercent& y, wci::Anchor anchor, int z)
{
    return impl->placeAdapter(id, wci::position(x, y), anchor, z);
}
wci::Id wci::Window::place(wci::DisplayAdapter& adapter, const wci::PositionSpec& position, wci::Anchor anchor, int z)
{
    return impl->placeAdapter(adapter, position, anchor, z);
}
wci::Id wci::Window::place(wci::DisplayAdapter&& adapter,const wci::PositionSpec& position,  wci::Anchor anchor, int z)
{
    return impl->placeAdapter(std::move(adapter), position, anchor, z);
}
wci::Id wci::Window::place(wci::Id id, const wci::PositionSpec& position, wci::Anchor anchor, int z)
{
    return impl->placeAdapter(id, position, anchor, z);
}

wci::Id wci::Window::place(wci::DisplayAdapter& child, wci::Id parentId, const wci::LenPercent& x, const wci::LenPercent& y, wci::Anchor anchor, int z)
{
    return impl->placeAdapter(child, parentId, wci::position(x, y), anchor, z);
}
wci::Id wci::Window::place(wci::DisplayAdapter&& child, wci::Id parentId, const wci::LenPercent& x, const wci::LenPercent& y, wci::Anchor anchor, int z)
{
    return impl->placeAdapter(std::move(child), parentId, wci::position(x, y), anchor, z);
}
wci::Id wci::Window::place(wci::Id childId, wci::Id parentId, const wci::LenPercent& x, const wci::LenPercent& y, wci::Anchor anchor, int z)
{
    return impl->placeAdapter(childId, parentId, wci::position(x, y), anchor, z);
}
wci::Id wci::Window::place(wci::DisplayAdapter& child, wci::Id parentId, const wci::PositionSpec& position, wci::Anchor anchor, int z)
{
    return impl->placeAdapter(child, parentId, position, anchor, z);
}
wci::Id wci::Window::place(wci::DisplayAdapter&& child, wci::Id parentId, const wci::PositionSpec& position, wci::Anchor anchor, int z)
{
    return impl->placeAdapter(std::move(child), parentId, position, anchor, z);
}
wci::Id wci::Window::place(wci::Id childId, wci::Id parentId, const wci::PositionSpec& position, wci::Anchor anchor, int z)
{
    return impl->placeAdapter(childId, parentId, position, anchor, z);
}

wci::Id wci::Window::place(wci::DisplayAdapter& child, wci::DisplayAdapter& parent, const wci::LenPercent& x, const wci::LenPercent& y, wci::Anchor anchor, int z)
{
    return impl->placeAdapter(child, impl->getId(parent), wci::position(x, y), anchor, z);
}
wci::Id wci::Window::place(wci::DisplayAdapter&& child, wci::DisplayAdapter& parent, const wci::LenPercent& x, const wci::LenPercent& y, wci::Anchor anchor, int z)
{
    return impl->placeAdapter(std::move(child), impl->getId(parent), wci::position(x, y), anchor, z);
}
wci::Id wci::Window::place(wci::Id childId, wci::DisplayAdapter& parent, const wci::LenPercent& x, const wci::LenPercent& y, wci::Anchor anchor, int z)
{
    return impl->placeAdapter(childId, impl->getId(parent), wci::position(x, y), anchor, z);
}
wci::Id wci::Window::place(wci::DisplayAdapter& child, wci::DisplayAdapter& parent, const wci::PositionSpec& position, wci::Anchor anchor, int z)
{
    return impl->placeAdapter(child, impl->getId(parent), position, anchor, z);
}
wci::Id wci::Window::place(wci::DisplayAdapter&& child, wci::DisplayAdapter& parent, const wci::PositionSpec& position, wci::Anchor anchor, int z)
{
    return impl->placeAdapter(std::move(child), impl->getId(parent), position, anchor, z);
}
wci::Id wci::Window::place(wci::Id childId, wci::DisplayAdapter& parent, const wci::PositionSpec& position, wci::Anchor anchor, int z)
{
    return impl->placeAdapter(childId, impl->getId(parent), position, anchor, z);
}

void wci::Window::unplace(wci::Id id)
{
    impl->unplaceAdapter(id);
}
void wci::Window::unplace(const wci::DisplayAdapter& adapter)
{
    impl->unplaceAdapter(adapter);
}

bool wci::Window::contains(wci::Id id) const noexcept
{
    return impl->adapterPlaced(id);
}
bool wci::Window::contains(const DisplayAdapter& adapter) const noexcept
{
    return impl->adapterPlaced(adapter);
}

void wci::Window::move(wci::Id id, const wci::LenPercent& x, const wci::LenPercent& y, wci::Anchor anchor, int z)
{
    impl->moveAdapter(id, wci::position(x, y), anchor, z);
}
void wci::Window::move(wci::Id id, const wci::PositionSpec& position, wci::Anchor anchor, int z)
{
    impl->moveAdapter(id, position, anchor, z);
}

void wci::Window::reposition(wci::Id id, const wci::PositionSpec& position)
{
    impl->repositionAdapter(id, position);
}

void wci::Window::reanchor(wci::Id id, wci::Anchor anchor)
{
    impl->reanchorAdapter(id, anchor);
}

void wci::Window::reorder(wci::Id id, int z)
{
    impl->reorderAdapter(id, z);
}

wci::DisplayAdapter& wci::Window::adapter(wci::Id id)
{
    return impl->getAdapter(id);
}
const wci::DisplayAdapter& wci::Window::adapter(wci::Id id) const
{
    return impl->getAdapter(id);
}

wci::Id wci::Window::id(const wci::DisplayAdapter& adapter) const noexcept
{
    return impl->getId(adapter);
}

wci::Point wci::Window::absolute(wci::Id id) const
{
    return impl->getAbsolute(id);
}

// const std::vector<wci::DisplayAdapter*>& wci::Window::children() const
// {
//     return impl->getRoots();
// }
// const std::vector<wci::DisplayAdapter*>& wci::Window::children(const wci::DisplayAdapter& adapter) const
// {
//     return impl->getChildren(adapter.id());
// }
// const std::vector<wci::DisplayAdapter*>& wci::Window::children(wci::Id id) const
// {
//     return impl->getChildren(id);
// }
