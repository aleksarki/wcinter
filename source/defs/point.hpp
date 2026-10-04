#pragma once
#ifndef WCI_SOURCE_DEFS_POINT_HPP
#define WCI_SOURCE_DEFS_POINT_HPP

#include "types.hpp"

namespace wci
{
    /*
     * Structure `Point` defines the coordinates of a character cell in  the console screen buffer.
     * The origin of the coordinate system (0; 0) is the upper-left cell of the buffer.
     */
    struct Point
    {
        Short x;
        Short y;

        constexpr bool operator==(const Point& other) const noexcept;

        constexpr bool operator!=(const Point& other) const noexcept;
    };
}

inline constexpr bool wci::Point::operator==(const wci::Point& other) const noexcept
{
    return x == other.x && y == other.y;
}

inline constexpr bool wci::Point::operator!=(const wci::Point& other) const noexcept
{
    return !(*this == other);
}

#endif  // WCI_SOURCE_DEFS_POINT_HPP
