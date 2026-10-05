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

        constexpr CharCell& operator<<=(Attribute other) noexcept;  // replace attribute in cell

        constexpr CharCell operator<<(Attribute other) const noexcept;  // replace attribute in cell

        constexpr CharCell& operator<<=(Wchar other) noexcept;  // replace character in cell

        constexpr CharCell operator<<(Wchar other) const noexcept;  // replace character in cell
    };

    constexpr CharCell operator+(Wchar character, Attribute attributes) noexcept;
}

inline constexpr bool wci::CharCell::operator==(const wci::CharCell& other) const noexcept
{
    return character == other.character && attributes == other.attributes;
}

inline constexpr bool wci::CharCell::operator!=(const wci::CharCell& other) const noexcept
{
    return !(*this == other);
}

inline constexpr wci::CharCell& wci::CharCell::operator<<=(wci::Attribute other) noexcept
{
    attributes = other;
    return *this;
}

inline constexpr wci::CharCell wci::CharCell::operator<<(wci::Attribute other) const noexcept
{
    return wci::CharCell{ character, other };
}

inline constexpr wci::CharCell& wci::CharCell::operator<<=(wci::Wchar other) noexcept
{
    character = other;
    return *this;
}

inline constexpr wci::CharCell wci::CharCell::operator<<(wci::Wchar other) const noexcept
{
    return wci::CharCell{ other, attributes };
}

inline constexpr wci::CharCell wci::operator+(wci::Wchar character, wci::Attribute attributes) noexcept
{
    return wci::CharCell{ character, attributes };
}

#endif  // WCI_SOURCE_DEFS_CHARCELL_HPP
