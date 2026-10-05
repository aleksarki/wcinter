#pragma once
#ifndef WCI_SOURCE_DEFS_RECT_HPP
#define WCI_SOURCE_DEFS_RECT_HPP

#include <algorithm>

#include "consts.hpp"
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

        constexpr bool within(const Rect& rect) const noexcept;

        constexpr bool intersects(const Rect& other) const noexcept;

        constexpr Rect intersect(const Rect& other) const noexcept;  // returns wci::norect when no intersection

        constexpr Rect& operator+=(const Point& other) noexcept;
        constexpr Rect& operator+=(Short other) noexcept;

        constexpr Rect& operator-=(const Point& other) noexcept;
        constexpr Rect& operator-=(Short other) noexcept;

        constexpr Rect operator+(const Point& other) const noexcept;
        constexpr Rect operator+(Short other) const noexcept;

        constexpr Rect operator-(const Point& other) const noexcept;
        constexpr Rect operator-(Short other) const noexcept;

        constexpr bool operator==(const Rect& other) const noexcept;

        constexpr bool operator!=(const Rect& other) const noexcept;
    };

    inline constexpr Rect norect{ 0, 0, -1, -1 };  // invalid/impossible rect
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

inline constexpr bool wci::Rect::within(const wci::Rect& rect) const noexcept
{
    return within(rect.topLeft()) && within(rect.bottomRight());
}

inline constexpr bool wci::Rect::intersects(const wci::Rect& other) const noexcept
{
    return (
        other.right > left && other.left < right &&
        other.bottom > top && other.top < bottom
    );
}

inline constexpr wci::Rect wci::Rect::intersect(const wci::Rect& other) const noexcept
{
    wci::Rect intersection{
        std::max(left, other.left),
        std::max(top, other.top),
        std::min(right, other.right),
        std::min(bottom, other.bottom)
    };
    if (!intersection.valid())
        return wci::norect;
    return intersection;
}

inline constexpr wci::Rect& wci::Rect::operator+=(const wci::Point& other) noexcept
{
    left += other.x;
    right += other.x;
    top += other.y;
    bottom += other.y;
    return *this;
}
inline constexpr wci::Rect& wci::Rect::operator+=(wci::Short other) noexcept
{
    left += other;
    right += other;
    top += other;
    bottom += other;
    return *this;
}

inline constexpr wci::Rect& wci::Rect::operator-=(const wci::Point& other) noexcept
{
    left -= other.x;
    right -= other.x;
    top -= other.y;
    bottom -= other.y;
    return *this;
}
inline constexpr wci::Rect& wci::Rect::operator-=(wci::Short other) noexcept
{
    left -= other;
    right -= other;
    top -= other;
    bottom -= other;
    return *this;
}

inline constexpr wci::Rect wci::Rect::operator+(const wci::Point& other) const noexcept
{
    auto result = *this;
    return result += other;
}
inline constexpr wci::Rect wci::Rect::operator+(wci::Short other) const noexcept
{
    auto result = *this;
    return result += other;
}

inline constexpr wci::Rect wci::Rect::operator-(const wci::Point& other) const noexcept
{
    auto result = *this;
    return result -= other;
}
inline constexpr wci::Rect wci::Rect::operator-(wci::Short other) const noexcept
{
    auto result = *this;
    return result -= other;
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

inline constexpr bool wci::Geometry::within(const wci::Rect& rect) const noexcept
{
    return toRect().within(rect);
}

inline constexpr wci::Rect wci::Geometry::toRect() const noexcept
{
    return wci::Rect{ 0, 0, width, height };
}

#endif  // WCI_SOURCE_DEFS_RECT_HPP
