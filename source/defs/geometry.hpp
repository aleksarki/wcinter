#pragma once
#ifndef WCI_SOURCE_DEFS_GEOMETRY_HPP
#define WCI_SOURCE_DEFS_GEOMETRY_HPP

#include "types.hpp"

namespace wci
{
    struct Geometry
    {
        Short width;
        Short height;

        constexpr bool operator==(const Geometry& other) const noexcept;
        constexpr bool operator!=(const Geometry& other) const noexcept;
    };
}

inline constexpr bool wci::Geometry::operator==(const wci::Geometry& other) const noexcept
{
    return width == other.width && height == other.height;
}

inline constexpr bool wci::Geometry::operator!=(const wci::Geometry& other) const noexcept
{
    return !(*this == other);
}

#endif  // WCI_SOURCE_DEFS_GEOMETRY_HPP
