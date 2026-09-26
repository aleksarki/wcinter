#pragma once
#ifndef WCI_SOURCE_CORE_WINDOW_HPP
#define WCI_SOURCE_CORE_WINDOW_HPP

#include <cstddef>
#include <memory>
#include <string>

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

        std::size_t place(const LenPercent& x, const LenPercent& y, Widget& widget, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const LenPercent& x, const LenPercent& y, Widget&& widget, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, Widget& widget, Anchor anchor = Anchor::TopLeft);
        std::size_t place(const PositionSpec& position, Widget&& widget, Anchor anchor = Anchor::TopLeft);

        void unplace(std::size_t id);
        void unplace(Widget& widget);

        bool placed(std::size_t id) const noexcept;
        bool placed(const Widget& widget) const noexcept;

        // todo implement placed widget by id; get by id

    private:
        class Impl;
        std::unique_ptr<Impl> impl;
    };
}

#endif  // WCI_SOURCE_CORE_WINDOW_HPP
