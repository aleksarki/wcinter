#pragma once
#ifndef WCI_SOURCE_CORE_WINDOW_HPP
#define WCI_SOURCE_CORE_WINDOW_HPP

#include <cstddef>
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

        // place on window
        std::size_t place(const LenPercent& x, const LenPercent& y, DisplayAdapter& adapter, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const LenPercent& x, const LenPercent& y, DisplayAdapter&& adapter, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const LenPercent& x, const LenPercent& y, std::size_t id, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, DisplayAdapter& adapter, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, DisplayAdapter&& adapter, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, std::size_t id, Anchor anchor = Anchor::TopLeft);

        // place on parent (by id)
        std::size_t place(const LenPercent& x, const LenPercent& y, std::size_t parentId, DisplayAdapter& child, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const LenPercent& x, const LenPercent& y, std::size_t parentId, DisplayAdapter&& child, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const LenPercent& x, const LenPercent& y, std::size_t parentId, std::size_t childId, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, std::size_t parentId, DisplayAdapter& child, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, std::size_t parentId, DisplayAdapter&& child, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, std::size_t parentId, std::size_t childId, Anchor anchor = Anchor::TopLeft);

        // place on parent (by reference)
        std::size_t place(const LenPercent& x, const LenPercent& y, DisplayAdapter& parent, DisplayAdapter& child, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const LenPercent& x, const LenPercent& y, DisplayAdapter& parent, DisplayAdapter&& child, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const LenPercent& x, const LenPercent& y, DisplayAdapter& parent, std::size_t childId, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, DisplayAdapter& parent, DisplayAdapter& child, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, DisplayAdapter& parent, DisplayAdapter&& child, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, DisplayAdapter& parent, std::size_t childId, Anchor anchor = Anchor::TopLeft);

        void move(const LenPercent& x, const LenPercent& y, DisplayAdapter& adapter, Anchor anchor = Anchor::TopLeft);
        void move(const LenPercent& x, const LenPercent& y, std::size_t id, Anchor anchor = Anchor::TopLeft);
        void move(const PositionSpec& position, DisplayAdapter& adapter, Anchor anchor = Anchor::TopLeft);
        void move(const PositionSpec& position, std::size_t id, Anchor anchor = Anchor::TopLeft);

        void unplace(std::size_t id);
        void unplace(DisplayAdapter& adapter);

        bool placed(std::size_t id) const noexcept;
        bool placed(const DisplayAdapter& adapter) const noexcept;

        DisplayAdapter& adapter(std::size_t id);
        const DisplayAdapter& adapter(std::size_t id) const;

        std::size_t id(const DisplayAdapter& adapter) const;

        // const std::vector<DisplayAdapter*>& children() const;
        // const std::vector<DisplayAdapter*>& children(const DisplayAdapter& adapter) const;
        // const std::vector<DisplayAdapter*>& children(std::size_t id) const;

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

        // idea implement widgetsAt(position)
        // todo implement blank(), fill(CharCell)
    };
}

#endif  // WCI_SOURCE_CORE_WINDOW_HPP
