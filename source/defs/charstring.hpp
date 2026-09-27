#pragma once
#ifndef WCI_SOURCE_DEFS_CHARSTRING_HPP
#define WCI_SOURCE_DEFS_CHARSTRING_HPP

#include <algorithm>
#include <cassert>
#include <climits>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "charmatrix.hpp"
#include "consts.hpp"
#include "enums.hpp"
#include "structs.hpp"
#include "types.hpp"

namespace wci
{
    class CharString;
    class CharMatrix;

    class CharString
    {
    private:
        struct {
            Short size;
            std::vector<CharInfo> string;
        } inner;

    public:
        CharString();
        CharString(Short size);
        CharString(Short size, Wchar character, Attribute attributes);
        CharString(Short size, const CharInfo& charInfo);
        CharString(const std::wstring& string);
        CharString(const std::wstring& string, Attribute attributes);
        CharString(const Wchar* string);
        CharString(const Wchar* string, Attribute attributes);
        CharString(const Wchar string[], Short length);
        CharString(const Wchar string[], Short length, Attribute attributes);

        CharInfo& at(Short i);
        const CharInfo& at(Short i) const;

        constexpr CharInfo& operator[](Short i) noexcept;
        constexpr const CharInfo& operator[](Short i) const noexcept;

        void put(Short i, Wchar character, Attribute attributes);
        void put(Short i, const CharInfo& charInfo);

        constexpr CharInfo* data() noexcept;
        constexpr const CharInfo* data() const noexcept;

        constexpr bool empty() const noexcept;

        constexpr Short size() const noexcept;

        void resize(Short size, Wchar character, Attribute attributes);
        void resize(Short size, const CharInfo& charInfo);
        void resize(Short size);

        void fill(Wchar character, Attribute attributes) noexcept;
        void fill(const CharInfo& charInfo) noexcept;

        void blank() noexcept;

        constexpr bool within(Short i) const noexcept;

        void swap(CharString& other) noexcept;
        friend void swap(CharString& a, CharString& b) noexcept;

    private:
        struct Intersection
        {
            Short baseStart, baseEnd;
            Short stackeeStart, stackeeEnd;
        };

        Intersection calculateIntersection(int base, int stackee, int offset) const noexcept;

    public:
        CharString overlay(Short offset, const CharString& charString) const;
        CharString overlay(const CharString& charString) const;

        // review what happens when str.inlay(str)?
        void inlay(Short offset, const CharString& charString);
        void inlay(const CharString& charString);

        // right point is exclusive
        CharString slice(Short i1, Short i2) const;

        void append(Wchar character, Attribute attributes);
        void append(const CharInfo& charInfo);
        void append(const CharString& string);

        void prepend(Wchar character, Attribute attributes);
        void prepend(const CharInfo& charInfo);
        void prepend(const CharString& string);

        CharString operator+(const CharString& other) const;

        CharString& operator+=(const CharString& other);

        constexpr bool operator==(const CharString& other) const;

        constexpr bool operator!=(const CharString& other) const;

        CharMatrix toMatrix() const;

        constexpr CharInfo* begin() noexcept;
        constexpr const CharInfo* begin() const noexcept;

        constexpr const CharInfo* cbegin() const noexcept;

        constexpr CharInfo* end() noexcept;
        constexpr const CharInfo* end() const noexcept;

        constexpr const CharInfo* cend() const noexcept;

        // idea write iterators for characters and attributes separately
        // idea write methods for setting text/attributes
    };
}

inline wci::CharString::CharString()
{
    resize(0);
}
inline wci::CharString::CharString(wci::Short size)
{
    if (size < 0)
        throw std::invalid_argument("CharString::CharString() got a negative size.");
    resize(size);
}
inline wci::CharString::CharString(wci::Short size, wci::Wchar character, wci::Attribute attributes)
{
    if (size < 0)
        throw std::invalid_argument("CharString::CharString() got a negative size.");
    resize(size, character, attributes);
}
inline wci::CharString::CharString(wci::Short size, const wci::CharInfo& charInfo)
{
    if (size < 0)
        throw std::invalid_argument("CharString::CharString() got a negative size.");
    resize(size, charInfo);
}
inline wci::CharString::CharString(const std::wstring& string)
{
    auto length = string.length();
    if (length > SHRT_MAX)
        throw std::length_error("CharString::CharString() got a string too long");
    resize(static_cast<wci::Short>(length));
    for (wci::Short i = 0; i < string.length(); ++i)
        inner.string[i] = wci::CharInfo{ string[i], wci::normal };
}
inline wci::CharString::CharString(const std::wstring& string, wci::Attribute attributes)
{
    auto length = string.length();
    if (length > SHRT_MAX)
        throw std::length_error("CharString::CharString() got a string too long");
    resize(static_cast<wci::Short>(length));
    for (wci::Short i = 0; i < string.length(); ++i)
        inner.string[i] = wci::CharInfo{ string[i], attributes };
}
inline wci::CharString::CharString(const wci::Wchar* string)
{
    std::size_t length = 0;
    for (; string[length]; ++length);  // get the length
    if (length > SHRT_MAX)
        throw std::length_error("CharString::CharString() got a string too long");
    resize(static_cast<wci::Short>(length));
    for (wci::Short i = 0; i < length; ++i)
        inner.string[i] = wci::CharInfo{ string[i], wci::normal };
}
inline wci::CharString::CharString(const wci::Wchar* string, wci::Attribute attributes)
{
    std::size_t length = 0;
    for (; string[length]; ++length);
    if (length > SHRT_MAX)
        throw std::length_error("CharString::CharString() got a string too long");
    resize(static_cast<wci::Short>(length));
    for (wci::Short i = 0; i < length; ++i)
        inner.string[i] = wci::CharInfo{ string[i], attributes };
}
inline wci::CharString::CharString(const wci::Wchar string[], wci::Short length)
{
    if (length < 0)
        throw std::invalid_argument("CharString::CharString() got a negative size.");
    resize(length);
    for (wci::Short i = 0; i < length; ++i)
        inner.string[i] = wci::CharInfo{ string[i], wci::normal };
}
inline wci::CharString::CharString(const wci::Wchar string[], wci::Short length, wci::Attribute attributes)
{
    if (length < 0)
        throw std::invalid_argument("CharString::CharString() got a negative size.");
    resize(length);
    for (wci::Short i = 0; i < length; ++i)
        inner.string[i] = wci::CharInfo{ string[i], attributes };
}

inline wci::CharInfo& wci::CharString::at(wci::Short i)
{
    assert(within(i));
    if (!within(i))
        throw std::out_of_range("CharString::at() position out of bounds");
    return inner.string[i];
}
inline const wci::CharInfo& wci::CharString::at(wci::Short i) const
{
    assert(within(i));
    if (!within(i))
        throw std::out_of_range("CharString::at() position out of bounds");
    return inner.string[i];
}

inline constexpr wci::CharInfo& wci::CharString::operator[](wci::Short i) noexcept
{
    return inner.string[i];
}
inline constexpr const wci::CharInfo& wci::CharString::operator[](wci::Short i) const noexcept
{
    return inner.string[i];
}

inline void wci::CharString::put(wci::Short i, wci::Wchar character, wci::Attribute attributes)
{
    put(i, wci::CharInfo{ character, attributes });
}
inline void wci::CharString::put(wci::Short i, const wci::CharInfo& charInfo)
{
    assert(within(i));
    if (!within(i))
        throw std::out_of_range("CharString::put() position out of bounds");
    inner.string[i] = charInfo;
}

inline constexpr wci::CharInfo* wci::CharString::data() noexcept
{
    return inner.string.data();
}
inline constexpr const wci::CharInfo* wci::CharString::data() const noexcept
{
    return inner.string.data();
}

inline constexpr bool wci::CharString::empty() const noexcept
{
    return inner.size == 0;
}

inline constexpr wci::Short wci::CharString::size() const noexcept
{
    return inner.size;
}

inline void wci::CharString::resize(wci::Short size, wci::Wchar character, wci::Attribute attributes)
{
    resize(size, wci::CharInfo{ character, attributes });
}
inline void wci::CharString::resize(wci::Short size, const wci::CharInfo& charInfo)
{
    if (size < 0)
        throw std::invalid_argument("CharString::resize() got a negative size.");
    inner.string.assign(size, charInfo);
    inner.size = size;
}
inline void wci::CharString::resize(wci::Short size)
{
    resize(size, wci::normalch);
}

inline void wci::CharString::fill(wci::Wchar character, wci::Attribute attributes) noexcept
{
    fill(wci::CharInfo{ character, attributes });
}
inline void wci::CharString::fill(const wci::CharInfo& charInfo) noexcept
{
    std::fill(begin(), end(), charInfo);
}

inline void wci::CharString::blank() noexcept
{
    fill(wci::normalch);
}

inline constexpr bool wci::CharString::within(wci::Short i) const noexcept
{
    return i >= 0 && i < inner.size;
}

inline void wci::CharString::swap(wci::CharString& other) noexcept
{
    std::swap(inner.size, other.inner.size);
    inner.string.swap(other.inner.string);
}
inline void wci::swap(wci::CharString& a, wci::CharString& b) noexcept
{
    a.swap(b);
}

inline wci::CharString::Intersection wci::CharString::calculateIntersection(int base, int stackee, int offset) const noexcept
{
    auto left = [](int baseLength, int offset) -> int
    {
        return std::max(std::min(baseLength, offset), 0);
    };
    auto right = [](int baseLength, int stackeeLength, int offset) -> int
    {
        return std::max(std::min(baseLength, stackeeLength + offset), 0);
    };
    return {
        .baseStart =    static_cast<wci::Short>(left (base, offset)),
        .baseEnd =      static_cast<wci::Short>(right(base, stackee, offset)),
        .stackeeStart = static_cast<wci::Short>(left (stackee, -offset)),
        .stackeeEnd =   static_cast<wci::Short>(right(stackee, base, -offset)),
    };
}

inline wci::CharString wci::CharString::overlay(wci::Short offset, const wci::CharString& charString) const
{
    auto isct = calculateIntersection(size(), charString.size(), offset);

    wci::CharString string = *this;
    auto dest = string.data();
    auto source = charString.data();

    for (wci::Short i = 0; isct.baseStart + i < isct.baseEnd; ++i)
        dest[isct.baseStart + i] = source[isct.stackeeStart + i];

    return string;
}
inline wci::CharString wci::CharString::overlay(const wci::CharString& charString) const
{
    return overlay(0, charString);
}

inline void wci::CharString::inlay(wci::Short offset, const wci::CharString& charString)
{
    auto isct = calculateIntersection(size(), charString.size(), offset);

    auto dest = data();
    auto source = charString.data();

    for (wci::Short i = 0; isct.baseStart + i < isct.baseEnd; ++i)
        dest[isct.baseStart + i] = source[isct.stackeeStart + i];
}
inline void wci::CharString::inlay(const wci::CharString& charString)
{
    inlay(0, charString);
}

inline wci::CharString wci::CharString::slice(wci::Short i1, wci::Short i2) const
{
    if (i1 < 0 || i1 > size() || i2 < 0 || i2 > size())
        throw std::invalid_argument("CharString::slice() got invalid coordinates");

    wci::Short width = i2 - i1;
    if (width < 0)
        throw std::invalid_argument("CharString::slice() got invalid coordinates");

    wci::CharString string(width);
    auto dest = string.data();
    auto source = data();

    for (wci::Short i = 0; i < width; ++i)
        dest[i] = source[i + i1];

    return string;
}

inline void wci::CharString::append(wci::Wchar character, wci::Attribute attributes)
{
    inner.string.emplace_back(character, attributes);
    ++inner.size;
}
inline void wci::CharString::append(const wci::CharInfo& charInfo)
{
    inner.string.push_back(charInfo);
    ++inner.size;
}
inline void wci::CharString::append(const wci::CharString& string)
{
    inner.string.insert(inner.string.end(), string.begin(), string.end());
    inner.size += string.size();
}

inline void wci::CharString::prepend(wci::Wchar character, wci::Attribute attributes)
{
    inner.string.insert(inner.string.begin(), wci::CharInfo{ character, attributes });
    ++inner.size;
}
inline void wci::CharString::prepend(const wci::CharInfo& charInfo)
{
    inner.string.insert(inner.string.begin(), charInfo);
    ++inner.size;
}
inline void wci::CharString::prepend(const wci::CharString& string)
{
    inner.string.insert(inner.string.begin(), string.begin(), string.end());
    inner.size += string.size();
}

inline wci::CharString wci::CharString::operator+(const wci::CharString& other) const
{
    wci::CharString string = *this;
    string.append(other);
    return string;
}

inline wci::CharString& wci::CharString::operator+=(const wci::CharString& other)
{
    append(other);
    return *this;
}

inline constexpr bool wci::CharString::operator==(const wci::CharString& other) const
{
    return inner.size == other.inner.size && inner.string == other.inner.string;
}

inline constexpr bool wci::CharString::operator!=(const wci::CharString& other) const
{
    return inner.size != other.inner.size || inner.string != other.inner.string;
}

inline wci::CharMatrix wci::CharString::toMatrix() const
{
    wci::CharMatrix matrix(size(), 1);
    auto dest = matrix.data();
    auto source = data();
    for (Short i = 0; i < size(); ++i)
        dest[i] = source[i];
    return matrix;
}

inline constexpr wci::CharInfo* wci::CharString::begin() noexcept
{
    return data();
}
inline constexpr const wci::CharInfo* wci::CharString::begin() const noexcept
{
    return data();
}

inline constexpr const wci::CharInfo* wci::CharString::cbegin() const noexcept
{
    return data();
}

inline constexpr wci::CharInfo* wci::CharString::end() noexcept
{
    return data() + size();
}
inline constexpr const wci::CharInfo* wci::CharString::end() const noexcept
{
    return data() + size();
}

inline constexpr const wci::CharInfo* wci::CharString::cend() const noexcept
{
    return data() + size();
}

#endif  // WCI_SOURCE_DEFS_CHARSTRING_HPP
