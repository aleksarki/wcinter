#pragma once
#ifndef WCI_SOURCE_DEFS_CONSTS_HPP
#define WCI_SOURCE_DEFS_CONSTS_HPP

#include <cstddef>

#include "enums.hpp"
#include "structs.hpp"
#include "types.hpp"

namespace wci
{
    inline constexpr struct Reset {} reset;
    inline constexpr Attribute normal = Attribute::BgColorBlack | Attribute::FgColorWhite;
    inline constexpr Wchar nline = L'\n';
    inline constexpr Wchar tab = L'\t';
    inline constexpr Wchar zero = L'\0';
    inline constexpr CharInfo normalch{ zero, normal };
    inline constexpr CharInfo nullch{ zero, Attribute::No };
    inline constexpr std::size_t npos(-1);
}

#endif  // WCI_SOURCE_DEFS_CONSTS_HPP
