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
    constexpr LenPercent operator""_a(unsigned long long value)  // alias for wci::LenPercent wci::literals::operator ""_abs
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
            throw std::invalid_argument("constexpr wci::LenPercent wci::literals::operator\"\"_rel(long double value) got invalid value");
        return LenPercent(static_cast<double>(value));
    }
    constexpr LenPercent operator""_rel(unsigned long long value)
    {
        if (!(0 <= value && value <= 100))
            throw std::invalid_argument("constexpr wci::LenPercent wci::literals::operator\"\"_rel(unsigned long long value) got invalid value");
        return LenPercent(static_cast<double>(value / 100.));
    }
    constexpr LenPercent operator""_r(long double value)  // alias for wci::LenPercent wci::literals::operator ""_rel
    {
        return operator""_rel(value);
    }
    constexpr LenPercent operator""_r(unsigned long long value)  // alias for wci::LenPercent wci::literals::operator ""_rel
    {
        return operator""_rel(value);
    }
}

#endif  // WCI_SOURCE_DEFS_LITERALS_HPP
