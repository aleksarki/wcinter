#pragma once
#ifndef WCI_SOURCE_DEFS_WIDGET_HPP
#define WCI_SOURCE_DEFS_WIDGET_HPP

#include <concepts>
#include <functional>

#include "charmatrix.hpp"
#include "enums.hpp"
#include "structs.hpp"
#include "types.hpp"

namespace wci
{
    class Window;

    template<typename T>
    concept renderable = requires(const T& t)
    {
        { t.toMatrix() } -> std::convertible_to<CharMatrix>;
        // { t.size() } -> std::convertible_to<Coord>;
    };

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
        // std::function<Coord()> sizer;

    public:
        template<renderable T>
        Widget(T& object);

        Widget(const Widget&) = delete;
        Widget& operator=(const Widget&) = delete;

        Widget(Widget&&) = default;
        Widget& operator=(Widget&&) = default;

        const PositionSpec& position() const noexcept;

        Anchor anchor() const noexcept;

        Coord absolute() const noexcept;

        bool visible() const noexcept;
        void visible(bool newVisibility) noexcept;

        bool placed() const noexcept;

        CharMatrix render() const;

    private:
        // these are private for now
        // todo make position() and absolute() public

        void position(const PositionSpec& newPosition) noexcept;

        void anchor(Anchor newAnchor) noexcept;

        void absolute(Coord newAbsolute) noexcept;

        void placed(bool hasBeenPlaced) noexcept;
    };
}

template<wci::renderable T>
inline wci::Widget::Widget(T& object) :
    inner{
        wci::PositionSpec{ wci::Short(0), wci::Short(0) },
        wci::Anchor::TopLeft,
        wci::Coord{ 0, 0 },
        true,
        false
    },
    renderer([&object]() -> wci::CharMatrix { return object.toMatrix(); })
{}

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
