#pragma once
#ifndef CINTER_INCLUDE_DEFINITIONS_TYPES_HPP
#define CINTER_INCLUDE_DEFINITIONS_TYPES_HPP

#include <climits>
#include <stdexcept>
#include <variant>

namespace wci
{
    using Wchar = wchar_t;
    using Short = short;
    using Word = unsigned short;
    using Dword = unsigned long;
    using Handle = void*;
    using LenPercent = std::variant<Short, double>;  // either absolute or relative coordinate

    constexpr LenPercent& operator+=(LenPercent& lp, const LenPercent& other)
    {
        if (std::holds_alternative<Short>(lp) && std::holds_alternative<Short>(other))
        {
            std::get<Short>(lp) += std::get<Short>(other);
            return lp;
        }
        else if (std::holds_alternative<double>(lp) && std::holds_alternative<double>(other))
        {
            std::get<double>(lp) += std::get<double>(other);
            return lp;
        }
        throw std::invalid_argument("constexpr wci::LenPercent& wci::operator+=(wci::LenPercent&, const wci::LenPercent&) got incompatible values");
    }
    constexpr LenPercent& operator+=(LenPercent& lp, int other)
    {
        if (std::holds_alternative<Short>(lp))
        {
            std::get<Short>(lp) += other;
            return lp;
        }
        throw std::invalid_argument("constexpr wci::LenPercent& wci::operator+=(wci::LenPercent&, int) got invalid value");
    }

    constexpr LenPercent& operator-=(LenPercent& lp, const LenPercent& other)
    {
        if (std::holds_alternative<Short>(lp) && std::holds_alternative<Short>(other))
        {
            std::get<Short>(lp) -= std::get<Short>(other);
            return lp;
        }
        else if (std::holds_alternative<double>(lp) && std::holds_alternative<double>(other))
        {
            std::get<double>(lp) -= std::get<double>(other);
            return lp;
        }
        throw std::invalid_argument("constexpr wci::LenPercent& wci::operator-=(wci::LenPercent&, const wci::LenPercent&) got incompatible values");
    }
    constexpr LenPercent& operator-=(LenPercent& lp, int other)
    {
        if (std::holds_alternative<Short>(lp))
        {
            std::get<Short>(lp) -= other;
            return lp;
        }
        throw std::invalid_argument("constexpr wci::LenPercent& wci::operator-=(wci::LenPercent&, int) got invalid value");
    }

    constexpr LenPercent& operator*=(LenPercent& lp, int other) noexcept
    {
        if (std::holds_alternative<Short>(lp))
        {
            std::get<Short>(lp) *= other;
            return lp;
        }
        std::get<double>(lp) *= other;
        return lp;
    }

    constexpr LenPercent& operator/=(LenPercent& lp, int other)  // fix check for overflow when other == -1
    {
        if (other == 0)
            throw std::invalid_argument("constexpr wci::LenPercent& wci::operator/=(wci::LenPercent&, int) got invalid value");
        if (std::holds_alternative<Short>(lp))
        {
            std::get<Short>(lp) /= other;
            return lp;
        }
        std::get<double>(lp) /= other;
        return lp;
    }

    constexpr LenPercent& operator%=(LenPercent& lp, int other)
    {
        if (other == 0)
            throw std::invalid_argument("constexpr wci::LenPercent& wci::operator%=(wci::LenPercent&, int) got invalid value");
        if (std::holds_alternative<Short>(lp))
        {
            std::get<Short>(lp) %= other;
            return lp;
        }
        throw std::invalid_argument("constexpr wci::LenPercent& wci::operator%=(wci::LenPercent&, int) got invalid value");
    }

    constexpr LenPercent operator+(const LenPercent& left, const LenPercent& right)
    {
        auto result = left;
        return result += right;
    }
    constexpr LenPercent operator+(const LenPercent& left, int right)
    {
        auto result = left;
        return result += right;
    }

    constexpr LenPercent operator-(const LenPercent& left, const LenPercent& right)
    {
        auto result = left;
        return result -= right;
    }
    constexpr LenPercent operator-(const LenPercent& left, int right)
    {
        auto result = left;
        return result -= right;
    }

    constexpr LenPercent operator*(const LenPercent& left, int right)
    {
        auto result = left;
        return result *= right;
    }

    constexpr LenPercent operator/(const LenPercent& left, int right)
    {
        auto result = left;
        return result /= right;
    }

    constexpr LenPercent operator%(const LenPercent& left, int right)
    {
        auto result = left;
        return result %= right;
    }

    constexpr LenPercent operator+(const LenPercent& lp) noexcept
    {
        return lp;
    }

    constexpr LenPercent operator-(const LenPercent& lp)
    {
        if (std::holds_alternative<Short>(lp))
        {
            Short value = std::get<Short>(lp);
            if (value == SHRT_MIN)
                throw std::overflow_error("constexpr wci::LenPercent wci::operator-(const wci::LenPercent&) got value overflowing on negation");
            return LenPercent(Short(-value));
        }
        // got double
        throw std::invalid_argument("constexpr wci::LenPercent wci::operator-(const wci::LenPercent&) got invalid value");
    }

    constexpr bool operator==(const LenPercent& a, const LenPercent& b) noexcept
    {
        if (std::holds_alternative<Short>(a) && std::holds_alternative<Short>(b))
            return std::get<Short>(a) == std::get<Short>(b);
        if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b))
            return std::get<double>(a) == std::get<double>(b);
        return false;
    }

    constexpr bool operator!=(const LenPercent& a, const LenPercent& b)
    {
        return !(a == b);
    }

    constexpr LenPercent& operator++(LenPercent& lp)  // prefix
    {
        if (std::holds_alternative<Short>(lp))
        {
            ++std::get<Short>(lp);
            return lp;
        }
        throw std::invalid_argument("constexpr wci::LenPercent& wci::operator++(wci::LenPercent&) got invalid value");
    }

    constexpr LenPercent& operator--(LenPercent& lp)  // prefix
    {
        if (std::holds_alternative<Short>(lp))
        {
            --std::get<Short>(lp);
            return lp;
        }
        throw std::invalid_argument("constexpr wci::LenPercent& wci::operator--(wci::LenPercent&) got invalid value");
    }

    constexpr LenPercent operator++(LenPercent& lp, int)  // postfix
    {
        if (std::holds_alternative<Short>(lp))
        {
            auto old = lp;
            ++std::get<Short>(lp);
            return old;
        }
        throw std::invalid_argument("constexpr wci::LenPercent& wci::operator++(wci::LenPercent&, int) got invalid value");
    }

    constexpr LenPercent operator--(LenPercent& lp, int)  // postfix
    {
        if (std::holds_alternative<Short>(lp))
        {
            auto old = lp;
            --std::get<Short>(lp);
            return old;
        }
        throw std::invalid_argument("constexpr wci::LenPercent& wci::operator--(wci::LenPercent&, int) got invalid value");
    }
}

#endif  // CINTER_INCLUDE_DEFINITIONS_TYPES_HPP
