#pragma once
#ifndef WCI_SOURCE_DEFS_RECT_HPP
#define WCI_SOURCE_DEFS_RECT_HPP

#include "geometry.hpp"
#include "point.hpp"
#include "types.hpp"

namespace wci
{
    /*
     * Structure `Rect` defines coordinates of upper-left and lower-right corners of a rectangle.
     */
    struct Rect
    {
        Short left;    // X of top left corner.
        Short top;     // Y of top left corner.
        Short right;   // X of bottom right corner.
        Short bottom;  // Y of bottom right corner.

        constexpr bool valid() const noexcept;

        constexpr Point topLeft() const noexcept;

        constexpr Point bottomRight() const noexcept;

        constexpr Geometry size() const noexcept;

        constexpr bool within(const Point& point) const noexcept;

        constexpr bool intersects(const Rect& other) const noexcept;

        constexpr Rect intersect(const Rect& other) const noexcept;  // todo implement; use CharMatrix::calculateIntersection(int, int, int, int, int, int)

        // operator-(Rect) ?

        constexpr bool operator==(const Rect& other) const noexcept;

        constexpr bool operator!=(const Rect& other) const noexcept;
    };
}

inline constexpr bool wci::Rect::valid() const noexcept
{
    return left < right && top < bottom;
}

inline constexpr wci::Point wci::Rect::topLeft() const noexcept
{
    return wci::Point{ left, top };
}

inline constexpr wci::Point wci::Rect::bottomRight() const noexcept
{
    return wci::Point{ right, bottom };
}

inline constexpr wci::Geometry wci::Rect::size() const noexcept
{
    auto&& val = bottomRight() - topLeft();
    return wci::Geometry{ val.x, val.y };
}

inline constexpr bool wci::Rect::within(const wci::Point& point) const noexcept
{
    return left <= point.x && point.x < right && top <= point.y && point.y < bottom;
}

inline constexpr bool wci::Rect::intersects(const wci::Rect& other) const noexcept
{
    return (
        other.bottomRight().x > topLeft().x && other.topLeft().x < bottomRight().x &&
        other.bottomRight().y > topLeft().y && other.topLeft().y < bottomRight().y
    );
}

inline constexpr bool wci::Rect::operator==(const wci::Rect &other) const noexcept
{
    return
    (
        left ==   other.left  &&
        top ==    other.top   &&
        right ==  other.right &&
        bottom == other.bottom
    );
}

inline constexpr bool wci::Rect::operator!=(const wci::Rect& other) const noexcept
{
    return !(*this == other);
}

inline constexpr wci::Point wci::Point::clamp(const wci::Rect& rect) const noexcept
{
    return clamp(rect.topLeft(), rect.bottomRight());
}

#endif  // WCI_SOURCE_DEFS_RECT_HPP
