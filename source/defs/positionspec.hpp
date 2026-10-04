#pragma once
#ifndef WCI_SOURCE_DEFS_POSITIONSPEC_HPP
#define WCI_SOURCE_DEFS_POSITIONSPEC_HPP

#include "lenpercent.hpp"

namespace wci
{
    struct PositionSpec
    {
        LenPercent x;
        LenPercent y;

        constexpr PositionSpec& operator+=(const PositionSpec& other);
        constexpr PositionSpec& operator+=(int other);

        constexpr PositionSpec& operator-=(const PositionSpec& other);
        constexpr PositionSpec& operator-=(int other);

        constexpr PositionSpec& operator*=(int other) noexcept;

        constexpr PositionSpec& operator/=(int other);

        constexpr PositionSpec& operator%=(int other);

        constexpr PositionSpec operator+(const PositionSpec& other) const;
        constexpr PositionSpec operator+(int other) const;

        constexpr PositionSpec operator-(const PositionSpec& other) const;
        constexpr PositionSpec operator-(int other) const;

        constexpr PositionSpec operator*(int other) const noexcept;

        constexpr PositionSpec operator/(int other) const;

        constexpr PositionSpec operator%(int other) const;

        constexpr PositionSpec operator+() const noexcept;

        constexpr PositionSpec operator-() const;

        constexpr bool operator==(const PositionSpec& other) const noexcept;

        constexpr bool operator!=(const PositionSpec& other) const noexcept;

        constexpr PositionSpec& operator++();

        constexpr PositionSpec& operator--();

        constexpr PositionSpec operator++(int);

        constexpr PositionSpec operator--(int);
    };
}

inline constexpr wci::PositionSpec& wci::PositionSpec::operator+=(const wci::PositionSpec& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

inline constexpr wci::PositionSpec& wci::PositionSpec::operator+=(int other)
{
    x += other;
    y += other;
    return *this;
}
inline constexpr wci::PositionSpec& wci::PositionSpec::operator-=(const wci::PositionSpec& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

inline constexpr wci::PositionSpec& wci::PositionSpec::operator-=(int other)
{
    x -= other;
    y -= other;
    return *this;
}

inline constexpr wci::PositionSpec& wci::PositionSpec::operator*=(int other) noexcept
{
    x *= other;
    y *= other;
    return *this;
}

inline constexpr wci::PositionSpec& wci::PositionSpec::operator/=(int other)
{
    x /= other;
    y /= other;
    return *this;
}

inline constexpr wci::PositionSpec& wci::PositionSpec::operator%=(int other)
{
    x %= other;
    y %= other;
    return *this;
}

inline constexpr wci::PositionSpec wci::PositionSpec::operator+(const wci::PositionSpec& other) const
{
    auto result = *this;
    return result += other;
}
inline constexpr wci::PositionSpec wci::PositionSpec::operator+(int other) const
{
    auto result = *this;
    return result += other;
}

inline constexpr wci::PositionSpec wci::PositionSpec::operator-(const wci::PositionSpec& other) const
{
    auto result = *this;
    return result -= other;
}
inline constexpr wci::PositionSpec wci::PositionSpec::operator-(int other) const
{
    auto result = *this;
    return result -= other;
}

inline constexpr wci::PositionSpec wci::PositionSpec::operator*(int other) const noexcept
{
    auto result = *this;
    return result *= other;
}

inline constexpr wci::PositionSpec wci::PositionSpec::operator/(int other) const
{
    auto result = *this;
    return result /= other;
}

inline constexpr wci::PositionSpec wci::PositionSpec::operator%(int other) const
{
    auto result = *this;
    return result %= other;
}

inline constexpr wci::PositionSpec wci::PositionSpec::operator+() const noexcept
{
    return *this;
}

inline constexpr wci::PositionSpec wci::PositionSpec::operator-() const
{
    return wci::PositionSpec{ -x, -y };
}

inline constexpr bool wci::PositionSpec::operator==(const wci::PositionSpec& other) const noexcept
{
    return x == other.x && y == other.y;
}

inline constexpr bool wci::PositionSpec::operator!=(const wci::PositionSpec& other) const noexcept
{
    return !(*this == other);
}

inline constexpr wci::PositionSpec& wci::PositionSpec::operator++()
{
    ++x;
    ++y;
    return *this;
}

inline constexpr wci::PositionSpec& wci::PositionSpec::operator--()
{
    --x;
    --y;
    return *this;
}

inline constexpr wci::PositionSpec wci::PositionSpec::operator++(int)
{
    auto old = *this;
    ++*this;
    return old;
}

inline constexpr wci::PositionSpec wci::PositionSpec::operator--(int)
{
    auto old = *this;
    --*this;
    return old;
}

#endif  // WCI_SOURCE_DEFS_POSITIONSPEC_HPP
