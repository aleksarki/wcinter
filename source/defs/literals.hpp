#pragma once
#ifndef WCI_SOURCE_DEFS_LITERALS_HPP
#define WCI_SOURCE_DEFS_LITERALS_HPP

#include <climits>
#include <stdexcept>
#include "types.hpp"

namespace wci::literals
{
    constexpr LenPercent operator""_abs(unsigned long long value)
    {
        if (value > SHRT_MAX)
            throw std::invalid_argument("constexpr wci::LenPercent wci::literals::operator\"\"_abs(unsigned long long) got invalid value");
        return LenPercent(static_cast<Short>(value));
    }
    constexpr LenPercent operator""_a(unsigned long long value)
    {
        return operator""_abs(value);
    }

    constexpr LenPercent operator""_absn(unsigned long long value)
    {
        if (value > -static_cast<long long>(SHRT_MIN))
            throw std::invalid_argument("constexpr wci::LenPercent wci::literals::operator\"\"_absn(unsigned long long) got invalid value");
        return LenPercent(static_cast<Short>(-static_cast<long long>(value)));
    }

    constexpr LenPercent operator""_rel(long double value)
    {
        if (!(0. <= value && value <= 1.))
            throw std::invalid_argument("wci::literals::operator\"\"_rel() got invalid value");
        return LenPercent(static_cast<double>(value));
    }
    constexpr LenPercent operator""_r(long double value)
    {
        return operator""_rel(value);
    }
}

#endif  // WCI_SOURCE_DEFS_LITERALS_HPP
