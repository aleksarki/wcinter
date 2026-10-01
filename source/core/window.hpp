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

        Coord size() const;

        void resize();  // idea rename to fit

        void render();

        // character
        void put(Short x, Short y, Wchar character);
        void put(Short x, Short y, Wchar character, Attribute attributes);
        void put(Short x, Short y, const CharInfo& charInfo);  // todo implement
        void put(const Coord& position, const CharInfo& charInfo);

        // string
        void put(Short x, Short y, const Wchar* string);
        void put(Short x, Short y, const Wchar* string, Attribute attributes);
        void put(Short x, Short y, const std::wstring& string);  // todo implement
        void put(const Coord& position, const CharInfo charInfos[], size_t length);
        void put(const Coord& position, const std::wstring& string);  // todo implement

        // matrix
        void put(Short x, Short y, const CharMatrix& matrix);
        void put(const Coord& position, const CharMatrix& matrix);

        // place on window
        std::size_t place(const LenPercent& x, const LenPercent& y, Widget& widget, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const LenPercent& x, const LenPercent& y, Widget&& widget, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const LenPercent& x, const LenPercent& y, std::size_t id, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, Widget& widget, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, Widget&& widget, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, std::size_t id, Anchor anchor = Anchor::TopLeft);

        // place on parent (by id)
        std::size_t place(const LenPercent& x, const LenPercent& y, std::size_t parentId, Widget& child, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const LenPercent& x, const LenPercent& y, std::size_t parentId, Widget&& child, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const LenPercent& x, const LenPercent& y, std::size_t parentId, std::size_t childId, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, std::size_t parentId, Widget& child, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, std::size_t parentId, Widget&& child, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, std::size_t parentId, std::size_t childId, Anchor anchor = Anchor::TopLeft);

        // place on parent (by reference)
        std::size_t place(const LenPercent& x, const LenPercent& y, Widget& parent, Widget& child, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const LenPercent& x, const LenPercent& y, Widget& parent, Widget&& child, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const LenPercent& x, const LenPercent& y, Widget& parent, std::size_t childId, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, Widget& parent, Widget& child, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, Widget& parent, Widget&& child, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, Widget& parent, std::size_t childId, Anchor anchor = Anchor::TopLeft);

        void move(const LenPercent& x, const LenPercent& y, Widget& widget, Anchor anchor = Anchor::TopLeft);
        void move(const LenPercent& x, const LenPercent& y, std::size_t id, Anchor anchor = Anchor::TopLeft);
        void move(const PositionSpec& position, Widget& widget, Anchor anchor = Anchor::TopLeft);
        void move(const PositionSpec& position, std::size_t id, Anchor anchor = Anchor::TopLeft);

        void unplace(std::size_t id);
        void unplace(Widget& widget);

        bool placed(std::size_t id) const noexcept;
        bool placed(const Widget& widget) const noexcept;

        Widget& widget(std::size_t id);
        const Widget& widget(std::size_t id) const;

        std::size_t id(const Widget& widget) const;

        const std::vector<Widget*>& children() const;
        const std::vector<Widget*>& children(const Widget& widget) const;
        const std::vector<Widget*>& children(std::size_t id) const;

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

        // idea implement widgetsAt(position)
        // todo implement blank(), fill(charInfo)
    };
}

#endif  // WCI_SOURCE_CORE_WINDOW_HPP
