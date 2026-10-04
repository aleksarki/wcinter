#pragma once
#ifndef WCI_SOURCE_DEFS_RECT_HPP
#define WCI_SOURCE_DEFS_RECT_HPP

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

        constexpr bool operator==(const Rect& other) const noexcept;

        constexpr bool operator!=(const Rect& other) const noexcept;
    };
}

inline constexpr bool wci::Rect::operator==(const wci::Rect& other) const noexcept
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

#endif  // WCI_SOURCE_DEFS_RECT_HPP
