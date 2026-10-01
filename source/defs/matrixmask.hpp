#pragma once
#ifndef WCI_SOURCE_DEFS_MATRIXMASK_HPP
#define WCI_SOURCE_DEFS_MATRIXMASK_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <vector>

#include "structs.hpp"
#include "types.hpp"

namespace wci
{
    class MatrixMask
    {
    private:
        struct {
            Coord size;
            std::vector<std::uint8_t> matrix;
        } inner;

    public:
        constexpr MatrixMask(std::uint8_t value = false8);
        constexpr MatrixMask(Short x, Short y, std::uint8_t value = false8);
        constexpr MatrixMask(const Coord& size, std::uint8_t value = false8);

        constexpr std::uint8_t& at(Short x, Short y);
        constexpr const std::uint8_t& at(Short x, Short y) const;
        constexpr std::uint8_t& at(const Coord& position);
        constexpr const std::uint8_t& at(const Coord& position) const;

        constexpr std::uint8_t& operator[](const Coord& position) noexcept;
        constexpr const std::uint8_t& operator[](const Coord& position) const noexcept;

        constexpr void set();  // set all
        constexpr void set(Short x, Short y);
        constexpr void set(const Coord& position);

        constexpr void clear();  // clear all
        constexpr void clear(Short x, Short y);
        constexpr void clear(const Coord& position);

        constexpr void flip();  // flip all
        constexpr void flip(Short x, Short y);
        constexpr void flip(const Coord& position);

        explicit operator bool() const noexcept;  // any value

        constexpr bool any() const noexcept;

        constexpr bool all() const noexcept;

        constexpr bool none() const noexcept;

        constexpr std::uint8_t* data() noexcept;
        constexpr const std::uint8_t* data() const noexcept;

        constexpr bool within(Short x, Short y) const noexcept;
        constexpr bool within(const Coord& position) const noexcept;

        constexpr bool empty() const noexcept;  // whether size is 0

        constexpr Coord size() const noexcept;

        // smallest rectangle containing all true values.
        // bottom-right point is exclusive.
        // returns std::nullopt if has no true values.
        constexpr std::optional<SmallRect> boundingBox() const;
    };
}

inline constexpr wci::MatrixMask::MatrixMask(std::uint8_t value) : MatrixMask(0, 0, value)
{}
inline constexpr wci::MatrixMask::MatrixMask(wci::Short x, wci::Short y, std::uint8_t value) :
    inner{ wci::Coord{ x, y }, std::vector<unsigned char>(x * y, static_cast<unsigned char>(value)) }
{
    if (x < 0 || y < 0)
        throw std::invalid_argument("MatrixMask::MatrixMask() got a negative dimension");
}
inline constexpr wci::MatrixMask::MatrixMask(const wci::Coord& size, std::uint8_t value) :
    inner{ size, std::vector<unsigned char>(size.x * size.y, static_cast<unsigned char>(value)) }
{
    if (size.x < 0 || size.y < 0)
        throw std::invalid_argument("MatrixMask::MatrixMask() got a negative dimension");
}

inline constexpr std::uint8_t& wci::MatrixMask::at(wci::Short x, wci::Short y)
{
    if (!within(x, y))
        throw std::out_of_range("MatrixMask::at() position out of bounds");
    return inner.matrix[y * inner.size.x + x];
}
inline constexpr const std::uint8_t& wci::MatrixMask::at(wci::Short x, wci::Short y) const
{
    if (!within(x, y))
        throw std::out_of_range("MatrixMask::at() position out of bounds");
    return inner.matrix[y * inner.size.x + x];
}
inline constexpr std::uint8_t& wci::MatrixMask::at(const wci::Coord& position)
{
    return at(position.x, position.y);
}
inline constexpr const std::uint8_t& wci::MatrixMask::at(const wci::Coord& position) const
{
    return at(position.x, position.y);
}

inline constexpr std::uint8_t& wci::MatrixMask::operator[](const wci::Coord& position) noexcept
{
    return inner.matrix[position.y * inner.size.x + position.x];
}
inline constexpr const std::uint8_t& wci::MatrixMask::operator[](const wci::Coord& position) const noexcept
{
    return inner.matrix[position.y * inner.size.x + position.x];
}

inline constexpr void wci::MatrixMask::set()
{
    std::fill(inner.matrix.begin(), inner.matrix.end(), wci::true8);
}
inline constexpr void wci::MatrixMask::set(wci::Short x, wci::Short y)
{
    if (!within(x, y))
        throw std::out_of_range("MatrixMask::set() position out of bounds");
    inner.matrix[y * inner.size.x + x] = wci::true8;
}
inline constexpr void wci::MatrixMask::set(const wci::Coord& position)
{
    set(position.x, position.y);
}

inline constexpr void wci::MatrixMask::clear()
{
    std::fill(inner.matrix.begin(), inner.matrix.end(), wci::false8);
}
inline constexpr void wci::MatrixMask::clear(wci::Short x, wci::Short y)
{
    if (!within(x, y))
        throw std::out_of_range("MatrixMask::clear() position out of bounds");
    inner.matrix[y * inner.size.x + x] = wci::false8;
}
inline constexpr void wci::MatrixMask::clear(const wci::Coord& position)
{
    clear(position.x, position.y);
}

inline constexpr void wci::MatrixMask::flip()
{
    auto* dest = inner.matrix.data();
    for (std::size_t i = 0; i < inner.size.x * inner.size.y; ++i)
        dest[i] = bool(dest[i]) ? wci::false8 : wci::true8;
}
inline constexpr void wci::MatrixMask::flip(wci::Short x, wci::Short y)
{
    if (!within(x, y))
        throw std::out_of_range("MatrixMask::flip() position out of bounds");
    auto* dest = inner.matrix.data();
    std::size_t i = y * inner.size.x + x;
    dest[i] = bool(dest[i]) ? wci::false8 : wci::true8;
}
inline constexpr void wci::MatrixMask::flip(const wci::Coord& position)
{
    flip(position.x, position.y);
}

inline wci::MatrixMask::operator bool() const noexcept
{
    return any();
}

inline constexpr bool wci::MatrixMask::any() const noexcept
{
    return std::find(inner.matrix.begin(), inner.matrix.end(), wci::true8) != inner.matrix.end();
}

inline constexpr bool wci::MatrixMask::all() const noexcept
{
    return std::find(inner.matrix.begin(), inner.matrix.end(), wci::false8) == inner.matrix.end();
}

inline constexpr bool wci::MatrixMask::none() const noexcept
{
    return std::find(inner.matrix.begin(), inner.matrix.end(), wci::true8) == inner.matrix.end();
}

inline constexpr std::uint8_t* wci::MatrixMask::data() noexcept
{
    return inner.matrix.data();
}
inline constexpr const std::uint8_t* wci::MatrixMask::data() const noexcept
{
    return inner.matrix.data();
}

inline constexpr bool wci::MatrixMask::within(wci::Short x, wci::Short y) const noexcept
{
    return x >= 0 && x < inner.size.x && y >= 0 && y < inner.size.y;
}
inline constexpr bool wci::MatrixMask::within(const wci::Coord& position) const noexcept
{
    return within(position.x, position.y);
}

inline constexpr bool wci::MatrixMask::empty() const noexcept
{
    return inner.size.x == 0 || inner.size.y == 0;
}

inline constexpr wci::Coord wci::MatrixMask::size() const noexcept
{
    return inner.size;
}

inline constexpr std::optional<wci::SmallRect> wci::MatrixMask::boundingBox() const
{
    wci::SmallRect box{ size().x, size().y, -1, -1 };
    const auto* source = data();
    bool found = false;
    for (wci::Short i = 0; i < size().y; ++i)
    {
        for (wci::Short j = 0; j < size().x; ++j)
        {
            if (bool(source[i * inner.size.x + j]))
            {
                box.left = std::min(box.left, j);
                box.top = std::min(box.top, i);
                box.right = std::max(box.right, j);
                box.bottom = std::max(box.bottom, i);
                found = true;
            }
        }
    }
    if (!found)
        return std::nullopt;
    ++box.right;
    ++box.bottom;
    return box;
}

#endif  // WCI_SOURCE_DEFS_MATRIXMASK_HPP
