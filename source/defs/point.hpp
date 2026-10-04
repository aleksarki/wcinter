#pragma once
#ifndef WCI_SOURCE_DEFS_POINT_HPP
#define WCI_SOURCE_DEFS_POINT_HPP

#include <algorithm>
#include <stdexcept>

#include "types.hpp"

namespace wci
{
    struct Rect;

    /*
     * Structure `Point` defines the coordinates of a character cell in  the console screen buffer.
     * The origin of the coordinate system (0; 0) is the upper-left cell of the buffer.
     */
    struct Point
    {
        Short x;
        Short y;

        constexpr Point clamp(const Point& min, const Point& max) const noexcept;
        constexpr Point clamp(const Rect& rect) const noexcept;  // nb defined in "rect.hpp"

        constexpr Point& operator+=(const Point& other) noexcept;
        constexpr Point& operator+=(Short other) noexcept;

        constexpr Point& operator-=(const Point& other) noexcept;
        constexpr Point& operator-=(Short other) noexcept;

        constexpr Point& operator*=(Short other) noexcept;
        constexpr Point& operator*=(double other) noexcept;

        constexpr Point& operator/=(Short other);
        constexpr Point& operator/=(double other);

        constexpr Point& operator%=(Short other);

        constexpr Point operator+(const Point& other) const noexcept;
        constexpr Point operator+(Short other) const noexcept;

        constexpr Point operator-(const Point& other) const noexcept;
        constexpr Point operator-(Short other) const noexcept;

        constexpr Point operator*(Short other) noexcept;
        constexpr Point operator*(double other) noexcept;

        constexpr Point operator/(Short other);
        constexpr Point operator/(double other);

        constexpr Point operator%(Short other);

        constexpr Point operator+() const noexcept;  // unary

        constexpr Point operator-() const noexcept;  // unary

        constexpr bool operator==(const Point& other) const noexcept;

        constexpr bool operator!=(const Point& other) const noexcept;

        constexpr Point& operator++() noexcept;  // prefix
        constexpr Point operator++(int) noexcept;  // postfix

        constexpr Point& operator--() noexcept;  // prefix
        constexpr Point operator--(int) noexcept;  // postfix
    };
}

inline constexpr wci::Point wci::Point::clamp(const wci::Point& min, const wci::Point& max) const noexcept
{
    return wci::Point{ std::min(std::max(x, min.x), max.x), std::min(std::max(y, min.y), max.y) };
}
// idea maybe unite point.hpp, geometry.hpp and rect.hpp?
// nb inline constexpr wci::Point wci::Point::clamp(const wci::Rect& rect) const noexcept; -> "rect.hpp"

inline constexpr wci::Point &wci::Point::operator+=(const wci::Point &other) noexcept
{
    x += other.x;
    y += other.y;
    return *this;
}
inline constexpr wci::Point& wci::Point::operator+=(wci::Short other) noexcept
{
    x += other;
    y += other;
    return *this;
}

inline constexpr wci::Point& wci::Point::operator-=(const wci::Point& other) noexcept
{
    x -= other.x;
    y -= other.y;
    return *this;
}
inline constexpr wci::Point& wci::Point::operator-=(wci::Short other) noexcept
{
    x -= other;
    y -= other;
    return *this;
}

inline constexpr wci::Point& wci::Point::operator*=(wci::Short other) noexcept
{
    x *= other;
    y *= other;
    return *this;
}
inline constexpr wci::Point& wci::Point::operator*=(double other) noexcept
{
    x = static_cast<wci::Short>(x * other);
    y = static_cast<wci::Short>(y * other);
    return *this;
}

inline constexpr wci::Point& wci::Point::operator/=(wci::Short other)
{
    if (other == 0)
        throw std::invalid_argument("constexpr wci::Point& wci::Point::operator/=(wci::Short) got invalid value");
    x /= other;
    y /= other;
    return *this;
}
inline constexpr wci::Point& wci::Point::operator/=(double other)
{
    if (other == .0)
        throw std::invalid_argument("constexpr wci::Point& wci::Point::operator/=(double) got invalid value");
    x = static_cast<wci::Short>(x / other);
    y = static_cast<wci::Short>(y / other);
    return *this;
}

inline constexpr wci::Point& wci::Point::operator%=(wci::Short other)
{
    if (other == 0)
        throw std::invalid_argument("constexpr wci::Point& wci::Point::operator%=(wci::Short) got invalid value");
    x %= other;
    y %= other;
    return *this;
}

inline constexpr wci::Point wci::Point::operator+(const wci::Point& other) const noexcept
{
    auto result = *this;
    return result += other;
}
inline constexpr wci::Point wci::Point::operator+(wci::Short other) const noexcept
{
    auto result = *this;
    return result += other;
}

inline constexpr wci::Point wci::Point::operator-(const wci::Point& other) const noexcept
{
    auto result = *this;
    return result -= other;
}
inline constexpr wci::Point wci::Point::operator-(wci::Short other) const noexcept
{
    auto result = *this;
    return result -= other;
}

inline constexpr wci::Point wci::Point::operator*(wci::Short other) noexcept
{
    auto result = *this;
    return result *= other;
}
inline constexpr wci::Point wci::Point::operator*(double other) noexcept
{
    auto result = *this;
    return result *= other;
}

inline constexpr wci::Point wci::Point::operator/(wci::Short other)
{
    auto result = *this;
    return result /= other;
}
inline constexpr wci::Point wci::Point::operator/(double other)
{
    auto result = *this;
    return result /= other;
}

inline constexpr wci::Point wci::Point::operator%(wci::Short other)
{
    auto result = *this;
    return result %= other;
}

inline constexpr wci::Point wci::Point::operator+() const noexcept
{
    return *this;
}

inline constexpr wci::Point wci::Point::operator-() const noexcept
{
    return wci::Point{ static_cast<wci::Short>(-x), static_cast<wci::Short>(-y) };
}

inline constexpr bool wci::Point::operator==(const wci::Point& other) const noexcept
{
    return x == other.x && y == other.y;
}

inline constexpr bool wci::Point::operator!=(const wci::Point& other) const noexcept
{
    return !(*this == other);
}

inline constexpr wci::Point& wci::Point::operator++() noexcept
{
    ++x;
    ++y;
    return *this;
}
inline constexpr wci::Point wci::Point::operator++(int) noexcept
{
    auto old = *this;
    ++*this;
    return old;
}

inline constexpr wci::Point& wci::Point::operator--() noexcept
{
    --x;
    --y;
    return *this;
}
inline constexpr wci::Point wci::Point::operator--(int) noexcept
{
    auto old = *this;
    --*this;
    return old;
}

#endif  // WCI_SOURCE_DEFS_POINT_HPP
