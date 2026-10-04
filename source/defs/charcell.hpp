#pragma once
#ifndef WCI_SOURCE_DEFS_CHARCELL_HPP
#define WCI_SOURCE_DEFS_CHARCELL_HPP

#include "enums.hpp"
#include "types.hpp"

namespace wci
{
    /*
     * Structure `CharCell` specifies a Unicode character and its attributes.
     * This structure is used by the console functions for reading from and writing to the console screen buffer.
     */
    struct CharCell
    {
        Wchar character;
        Attribute attributes;

        constexpr bool operator==(const CharCell& other) const noexcept;

        constexpr bool operator!=(const CharCell& other) const noexcept;
    };
}

inline constexpr bool wci::CharCell::operator==(const wci::CharCell& other) const noexcept
{
    return character == other.character && attributes == other.attributes;
}

inline constexpr bool wci::CharCell::operator!=(const wci::CharCell& other) const noexcept
{
    return !(*this == other);
}

#endif  // WCI_SOURCE_DEFS_CHARCELL_HPP
