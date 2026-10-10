#pragma once
#ifndef WCI_SOURCE_CORE_WINDOW_HPP
#define WCI_SOURCE_CORE_WINDOW_HPP

#include <memory>
#include <string>
#include <vector>

#include "../defs.hpp"
#include "console.hpp"

namespace wci {

    class Window
    {
    public:
        Window();
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        Console& console() noexcept;
        const Console& console() const noexcept;

        CharMatrix& matrix() noexcept;
        const CharMatrix& matrix() const noexcept;

        Geometry size() const;

        void refit();

        void render();

        void fill(Wchar character, Attribute attributes);
        void fill(const CharCell& cell);

        void blank();

        // character
        void put(Short x, Short y, Wchar character);
        void put(Short x, Short y, Wchar character, Attribute attributes);
        void put(Short x, Short y, const CharCell& cell);
        void put(const Point& position, const CharCell& cell);

        // string
        void put(Short x, Short y, const Wchar* string);
        void put(Short x, Short y, const Wchar* string, Attribute attributes);
        void put(Short x, Short y, const std::wstring& string);
        void put(const Point& position, const CharCell cells[], Short length);
        void put(const Point& position, const std::wstring& string);

        // matrix
        void put(Short x, Short y, const CharMatrix& matrix);
        void put(const Point& position, const CharMatrix& matrix);

        // todo change signatures

        // place on window
        Id place(const LenPercent& x, const LenPercent& y, DisplayAdapter& adapter, Anchor anchor = Anchor::TopLeft);
        Id place(const LenPercent& x, const LenPercent& y, DisplayAdapter&& adapter, Anchor anchor = Anchor::TopLeft);
        Id place(const LenPercent& x, const LenPercent& y, Id id, Anchor anchor = Anchor::TopLeft);
        Id place(const PositionSpec& position, DisplayAdapter& adapter, Anchor anchor = Anchor::TopLeft);
        Id place(const PositionSpec& position, DisplayAdapter&& adapter, Anchor anchor = Anchor::TopLeft);
        Id place(const PositionSpec& position, Id id, Anchor anchor = Anchor::TopLeft);

        // place on parent (by id)
        Id place(const LenPercent& x, const LenPercent& y, Id parentId, DisplayAdapter& child, Anchor anchor = Anchor::TopLeft);
        Id place(const LenPercent& x, const LenPercent& y, Id parentId, DisplayAdapter&& child, Anchor anchor = Anchor::TopLeft);
        Id place(const LenPercent& x, const LenPercent& y, Id parentId, Id childId, Anchor anchor = Anchor::TopLeft);
        Id place(const PositionSpec& position, Id parentId, DisplayAdapter& child, Anchor anchor = Anchor::TopLeft);
        Id place(const PositionSpec& position, Id parentId, DisplayAdapter&& child, Anchor anchor = Anchor::TopLeft);
        Id place(const PositionSpec& position, Id parentId, Id childId, Anchor anchor = Anchor::TopLeft);

        // place on parent (by reference)
        Id place(const LenPercent& x, const LenPercent& y, DisplayAdapter& parent, DisplayAdapter& child, Anchor anchor = Anchor::TopLeft);
        Id place(const LenPercent& x, const LenPercent& y, DisplayAdapter& parent, DisplayAdapter&& child, Anchor anchor = Anchor::TopLeft);
        Id place(const LenPercent& x, const LenPercent& y, DisplayAdapter& parent, Id childId, Anchor anchor = Anchor::TopLeft);
        Id place(const PositionSpec& position, DisplayAdapter& parent, DisplayAdapter& child, Anchor anchor = Anchor::TopLeft);
        Id place(const PositionSpec& position, DisplayAdapter& parent, DisplayAdapter&& child, Anchor anchor = Anchor::TopLeft);
        Id place(const PositionSpec& position, DisplayAdapter& parent, Id childId, Anchor anchor = Anchor::TopLeft);

        void unplace(Id id);
        void unplace(const DisplayAdapter& adapter);

        bool contains(Id id) const noexcept;
        bool contains(const DisplayAdapter& adapter) const noexcept;

        void move(const LenPercent& x, const LenPercent& y, DisplayAdapter& adapter, Anchor anchor = Anchor::TopLeft);
        void move(const LenPercent& x, const LenPercent& y, Id id, Anchor anchor = Anchor::TopLeft);
        void move(const PositionSpec& position, DisplayAdapter& adapter, Anchor anchor = Anchor::TopLeft);
        void move(const PositionSpec& position, Id id, Anchor anchor = Anchor::TopLeft);

        void reposition(Id id, const PositionSpec& position);

        void reanchor(Id id, Anchor anchor);

        void reorder(Id id, int z);

        DisplayAdapter& adapter(Id id);
        const DisplayAdapter& adapter(Id id) const;

        Id id(const DisplayAdapter& adapter) const;

        Point absolute(Id id) const;

        // const std::vector<DisplayAdapter*>& children() const;
        // const std::vector<DisplayAdapter*>& children(const DisplayAdapter& adapter) const;
        // const std::vector<DisplayAdapter*>& children(Id id) const;

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

        // idea implement widgetsAt(position)
        // todo implement blank(), fill(CharCell)
    };
}

#endif  // WCI_SOURCE_CORE_WINDOW_HPP
