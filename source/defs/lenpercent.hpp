#pragma once
#ifndef WCI_SOURCE_DEFS_LENPERCENT_HPP
#define WCI_SOURCE_DEFS_LENPERCENT_HPP

#include <climits>
#include <stdexcept>
#include <variant>

#include "types.hpp"

namespace wci
{
    // either absolute or relative coordinate
    class LenPercent
    {
    private:
        std::variant<Short, double> value;

    public:
        constexpr explicit LenPercent(Short sh);
        constexpr explicit LenPercent(double d);

        constexpr bool isAbsolute() const noexcept;

        constexpr bool isRelative() const noexcept;

        constexpr Short& absolute();
        constexpr Short absolute() const;

        constexpr double& relative();
        constexpr double relative() const;

        constexpr LenPercent& operator+=(const LenPercent& other);
        constexpr LenPercent& operator+=(int other);

        constexpr LenPercent& operator-=(const LenPercent& other);
        constexpr LenPercent& operator-=(int other);

        constexpr LenPercent& operator*=(int other) noexcept;

        constexpr LenPercent& operator/=(int other);

        constexpr LenPercent& operator%=(int other);

        constexpr LenPercent operator+(const LenPercent& other) const;
        constexpr LenPercent operator+(int other) const;

        constexpr LenPercent operator-(const LenPercent& other) const;
        constexpr LenPercent operator-(int other) const;

        constexpr LenPercent operator*(int other) const noexcept;

        constexpr LenPercent operator/(int other) const;

        constexpr LenPercent operator%(int other) const;

        constexpr LenPercent operator+() const noexcept;  // unary

        constexpr LenPercent operator-() const;  // unary

        constexpr bool operator==(const LenPercent& other) const noexcept;

        constexpr bool operator!=(const LenPercent& other) const noexcept;

        constexpr LenPercent& operator++();  // prefix

        constexpr LenPercent& operator--();  // prefix

        constexpr LenPercent operator++(int);  // postfix

        constexpr LenPercent operator--(int);  // postfix
    };

}

inline constexpr explicit wci::LenPercent::LenPercent(wci::Short sh) : value(sh)
{}

inline constexpr explicit wci::LenPercent::LenPercent(double d) : value(d)
{}

inline constexpr bool wci::LenPercent::isAbsolute() const noexcept
{
    return std::holds_alternative<Short>(value);
}

inline constexpr bool wci::LenPercent::isRelative() const noexcept
{
    return std::holds_alternative<double>(value);
}

inline constexpr wci::Short& wci::LenPercent::absolute()
{
    return std::get<Short>(value);
}

inline constexpr wci::Short wci::LenPercent::absolute() const
{
    return std::get<Short>(value);
}

inline constexpr double& wci::LenPercent::relative()
{
    return std::get<double>(value);
}

inline constexpr double wci::LenPercent::relative() const
{
    return std::get<double>(value);
}

inline constexpr wci::LenPercent& wci::LenPercent::operator+=(const wci::LenPercent& other)
{
    if (isAbsolute() && other.isAbsolute())
    {
        absolute() += other.absolute();
        return *this;
    }
    else if (isRelative() && other.isRelative())
    {
        relative() += other.relative();
        return *this;
    }
    throw std::invalid_argument("constexpr wci::LenPercent& wci::operator+=(wci::LenPercent&, const wci::LenPercent&) got incompatible values");
}
inline constexpr wci::LenPercent& wci::LenPercent::operator+=(int other)
{
    if (isAbsolute())
    {
        absolute() += other;
        return *this;
    }
    throw std::invalid_argument("constexpr wci::LenPercent& wci::operator+=(wci::LenPercent&, int) got invalid value");
}

inline constexpr wci::LenPercent& wci::LenPercent::operator-=(const wci::LenPercent& other)
{
    if (isAbsolute() && other.isAbsolute())
    {
        absolute() -= other.absolute();
        return *this;
    }
    else if (isRelative() && other.isRelative())
    {
        relative() -= other.relative();
        return *this;
    }
    throw std::invalid_argument("constexpr wci::LenPercent& wci::operator-=(wci::LenPercent&, const wci::LenPercent&) got incompatible values");
}
inline constexpr wci::LenPercent& wci::LenPercent::operator-=(int other)
{
    if (isAbsolute())
    {
        absolute() -= other;
        return *this;
    }
    throw std::invalid_argument("constexpr wci::LenPercent& wci::operator-=(wci::LenPercent&, int) got invalid value");
}

inline constexpr wci::LenPercent& wci::LenPercent::operator*=(int other) noexcept
{
    if (isAbsolute())
    {
        absolute() *= other;
        return *this;
    }
    relative() *= other;
    return *this;
}

inline constexpr wci::LenPercent& wci::LenPercent::operator/=(int other)  // fix check for overflow (when other == -1)
{
    if (other == 0)
        throw std::invalid_argument("constexpr wci::LenPercent& wci::operator/=(wci::LenPercent&, int) got invalid value");
    if (isAbsolute())
    {
        absolute() /= other;
        return *this;
    }
    relative() /= other;
    return *this;
}

inline constexpr wci::LenPercent& wci::LenPercent::operator%=(int other)
{
    if (other == 0)
        throw std::invalid_argument("constexpr wci::LenPercent& wci::operator%=(wci::LenPercent&, int) got invalid value");
    if (isAbsolute())
    {
        absolute() %= other;
        return *this;
    }
    throw std::invalid_argument("constexpr wci::LenPercent& wci::operator%=(wci::LenPercent&, int) got invalid value");
}

inline constexpr wci::LenPercent wci::LenPercent::operator+(const wci::LenPercent& other) const
{
    auto result = *this;
    return result += other;
}
inline constexpr wci::LenPercent wci::LenPercent::operator+(int other) const
{
    auto result = *this;
    return result += other;
}

inline constexpr wci::LenPercent wci::LenPercent::operator-(const wci::LenPercent& other) const
{
    auto result = *this;
    return result -= other;
}
inline constexpr wci::LenPercent wci::LenPercent::operator-(int other) const
{
    auto result = *this;
    return result -= other;
}

inline constexpr wci::LenPercent wci::LenPercent::operator*(int other) const noexcept
{
    auto result = *this;
    return result *= other;
}

inline constexpr wci::LenPercent wci::LenPercent::operator/(int other) const
{
    auto result = *this;
    return result /= other;
}

inline constexpr wci::LenPercent wci::LenPercent::operator%(int other) const
{
    auto result = *this;
    return result %= other;
}

inline constexpr wci::LenPercent wci::LenPercent::operator+() const noexcept
{
    return *this;
}

inline constexpr wci::LenPercent wci::LenPercent::operator-() const
{
    if (isAbsolute())
    {
        if (absolute() == SHRT_MIN)
            throw std::overflow_error("constexpr wci::LenPercent wci::operator-(const wci::LenPercent&) got value overflowing on negation");
        return LenPercent(Short(-absolute()));
    }
    // got double
    throw std::invalid_argument("constexpr wci::LenPercent wci::operator-(const wci::LenPercent&) got invalid value");
}

inline constexpr bool wci::LenPercent::operator==(const wci::LenPercent& other) const noexcept
{
    if (isAbsolute() && other.isAbsolute())
        return absolute() == other.absolute();
    if (isRelative() && other.isRelative())
        return relative() == other.relative();
    return false;
}

inline constexpr bool wci::LenPercent::operator!=(const wci::LenPercent& other) const noexcept
{
    return !(*this == other);
}

inline constexpr wci::LenPercent& wci::LenPercent::operator++()
{
    if (isAbsolute())
    {
        ++absolute();
        return *this;
    }
    throw std::invalid_argument("constexpr wci::LenPercent& wci::operator++(wci::LenPercent&) got invalid value");
}

inline constexpr wci::LenPercent& wci::LenPercent::operator--()
{
    if (isAbsolute())
    {
        --absolute();
        return *this;
    }
    throw std::invalid_argument("constexpr wci::LenPercent& wci::operator--(wci::LenPercent&) got invalid value");
}

inline constexpr wci::LenPercent wci::LenPercent::operator++(int)
{
    if (isAbsolute())
    {
        auto old = *this;
        ++*this;
        return old;
    }
    throw std::invalid_argument("constexpr wci::LenPercent& wci::operator++(wci::LenPercent&, int) got invalid value");
}

inline constexpr wci::LenPercent wci::LenPercent::operator--(int)
{
    if (isAbsolute())
    {
        auto old = *this;
        --*this;
        return old;
    }
    throw std::invalid_argument("constexpr wci::LenPercent& wci::operator--(wci::LenPercent&, int) got invalid value");
}

#endif  // WCI_SOURCE_DEFS_LENPERCENT_HPP
