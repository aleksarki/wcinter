#pragma once
#ifndef WCI_SOURCE_DEFS_GEOMETRY_HPP
#define WCI_SOURCE_DEFS_GEOMETRY_HPP

#include "point.hpp"
#include "types.hpp"

namespace wci
{
    struct Geometry
    {
        Short width;
        Short height;

        constexpr Short area() const noexcept;

        constexpr bool within(const Point& point) const noexcept;

        constexpr bool operator==(const Geometry& other) const noexcept;

        constexpr bool operator!=(const Geometry& other) const noexcept;
    };
}

inline constexpr wci::Short wci::Geometry::area() const noexcept
{
    return width * height;
}

inline constexpr bool wci::Geometry::within(const wci::Point& point) const noexcept
{
    return 0 <= point.x && point.x < width && 0 <= point.y && point.y < height;
}

inline constexpr bool wci::Geometry::operator==(const wci::Geometry &other) const noexcept
{
    return width == other.width && height == other.height;
}

inline constexpr bool wci::Geometry::operator!=(const wci::Geometry& other) const noexcept
{
    return !(*this == other);
}

#endif  // WCI_SOURCE_DEFS_GEOMETRY_HPP
