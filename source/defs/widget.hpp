#pragma once
#ifndef WCI_SOURCE_DEFS_WIDGET_HPP
#define WCI_SOURCE_DEFS_WIDGET_HPP

#include <concepts>
#include <functional>
#include <memory>
#include <string>

#include "charmatrix.hpp"
#include "charstring.hpp"
#include "enums.hpp"
#include "lenpercent.hpp"
#include "structs.hpp"
#include "types.hpp"

namespace wci
{
    class Window;

    template<typename T>
    concept renderable = requires(const T& t)
    {
        { t.toMatrix() } -> std::convertible_to<CharMatrix>;
        { t.size() } -> std::convertible_to<Coord>;
    };

    // NB: if widget constructs from an lvalue-object, the object must outlive the widget!
    class Widget
    {
        friend class Window;

    private:
        struct {
            PositionSpec position;
            Anchor anchor;
            Coord absolute;  // calculated position of the top left point
            bool visible;
            bool placed;
        } inner;
        std::function<CharMatrix()> renderer;
        std::function<Coord()> sizer;

        // ctor for delegating and filling inner struct with default values
        Widget(std::function<CharMatrix()> renderer, std::function<Coord()> sizer);

    public:
        template<renderable R>
        Widget(R& object);
        Widget(Wchar character);
        Widget(const CharInfo& charInfo);
        Widget(CharInfo&& charInfo);
        Widget(const std::wstring& string);
        Widget(std::wstring&& string);
        Widget(const CharString& charString);
        Widget(CharString&& charString);
        Widget(CharMatrix&& charMatrix);

        Widget(const Widget&) = delete;
        Widget& operator=(const Widget&) = delete;

        Widget(Widget&&) = default;
        Widget& operator=(Widget&&) = default;

        Coord size() const noexcept;

        const PositionSpec& position() const noexcept;

        Anchor anchor() const noexcept;

        Coord absolute() const noexcept;

        bool visible() const noexcept;
        void visible(bool newVisibility) noexcept;

        bool placed() const noexcept;

        CharMatrix render() const;

    private:
        // these are private for now
        // todo make position() and absolute() public through Window

        void position(const PositionSpec& newPosition) noexcept;

        void anchor(Anchor newAnchor) noexcept;

        void absolute(Coord newAbsolute) noexcept;

        void placed(bool hasBeenPlaced) noexcept;
    };
}

inline wci::Widget::Widget(std::function<wci::CharMatrix()> renderer, std::function<wci::Coord()> sizer) :
    inner{
        wci::PositionSpec{ wci::LenPercent(wci::Short(0)), wci::LenPercent(wci::Short(0)) },
        wci::Anchor::TopLeft,
        wci::Coord{ 0, 0 },
        true,
        false
    },
    renderer(std::move(renderer)),
    sizer(std::move(sizer))
{}
template<wci::renderable R>
inline wci::Widget::Widget(R& object) : wci::Widget(
    [&object]() -> wci::CharMatrix { return object.toMatrix(); },
    [&object]() -> wci::Coord { return object.size(); }
)
{}
inline wci::Widget::Widget(wci::Wchar character) : wci::Widget(
    [matrix = wci::CharMatrix(wci::Coord{ 1, 1 }, wci::CharInfo{ character, wci::normal })]() -> wci::CharMatrix { return matrix; },
    [size = wci::Coord{ 1, 1 }]() -> wci::Coord { return size; }
)
{}
inline wci::Widget::Widget(const CharInfo& charInfo) : wci::Widget(
    [&charInfo]() -> wci::CharMatrix { return wci::CharMatrix(wci::Coord{ 1, 1 }, charInfo); },
    [size = wci::Coord{ 1, 1 }]() -> wci::Coord { return size; }
)
{}
inline wci::Widget::Widget(wci::CharInfo&& charInfo) : wci::Widget(
    [matrix = wci::CharMatrix(wci::Coord{ 1, 1 }, charInfo)]() -> wci::CharMatrix { return matrix; },
    [size = wci::Coord{ 1, 1 }]() -> wci::Coord { return size; }
)
{}
inline wci::Widget::Widget(const std::wstring& string) : wci::Widget(
    [&string]() -> wci::CharMatrix { return wci::CharString(string).toMatrix(); },
    [&string]() -> wci::Coord { return wci::Coord{ wci::CharString(string).size(), 1 }; }
)
{}
inline wci::Widget::Widget(std::wstring&& string) : wci::Widget(
    [matrix = wci::CharString(string).toMatrix()]() -> wci::CharMatrix { return matrix; },
    [size = wci::Coord{ wci::CharString(string).size(), 1 }]() -> wci::Coord { return size; }
)
{}
inline wci::Widget::Widget(const CharString& charString) : wci::Widget (
    [&charString]() -> wci::CharMatrix { return charString.toMatrix(); },
    [&charString]() -> wci::Coord { return wci::Coord{ charString.size(), 1 }; }
)
{}
inline wci::Widget::Widget(CharString&& charString) : wci::Widget(
    [matrix = charString.toMatrix()]() -> wci::CharMatrix { return matrix; },
    [size = wci::Coord{ charString.size(), 1 }]() -> wci::Coord { return size; }
)
{}
inline wci::Widget::Widget(CharMatrix&& charMatrix) : wci::Widget(
    [charMatrix]() -> wci::CharMatrix { return charMatrix; },
    [size = charMatrix.size()]() -> wci::Coord { return size; }
)
{}

inline wci::Coord wci::Widget::size() const noexcept
{
    return sizer();
}

inline const wci::PositionSpec& wci::Widget::position() const noexcept
{
    return inner.position;
}
inline void wci::Widget::position(const wci::PositionSpec& newPosition) noexcept
{
    inner.position = newPosition;
}

inline wci::Anchor wci::Widget::anchor() const noexcept
{
    return inner.anchor;
}
inline void wci::Widget::anchor(wci::Anchor newAnchor) noexcept
{
    inner.anchor = newAnchor;
}

inline wci::Coord wci::Widget::absolute() const noexcept
{
    return inner.absolute;
}
inline void wci::Widget::absolute(wci::Coord newAbsolute) noexcept
{
    inner.absolute = newAbsolute;
}

inline bool wci::Widget::visible() const noexcept
{
    return inner.visible;
}
inline void wci::Widget::visible(bool newVisibility) noexcept
{
    inner.visible = newVisibility;
}

inline bool wci::Widget::placed() const noexcept
{
    return inner.placed;
}
inline void wci::Widget::placed(bool hasBeenPlaced) noexcept
{
    inner.placed = hasBeenPlaced;
}

inline wci::CharMatrix wci::Widget::render() const
{
    return renderer();
}

#endif  // WCI_SOURCE_DEFS_WIDGET_HPP
