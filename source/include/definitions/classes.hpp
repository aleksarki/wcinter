#pragma once
#ifndef CINTER_INCLUDE_DEFINITIONS_CLASSES_HPP
#define CINTER_INCLUDE_DEFINITIONS_CLASSES_HPP

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

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
        static constexpr CharInfo nullChar{ 0, Attribute::No };
        static constexpr Attribute stdAttr = Attribute::BgColorBlack | Attribute::FgColorWhite;

        CharString()
        {
            resize(0);
        }
        CharString(Short size)
        {
            if (size < 0)
                throw std::invalid_argument("CharString::CharString() got a negative size.");
            resize(size);
        }
        CharString(Short size, Wchar character, Attribute attributes)
        {
            if (size < 0)
                throw std::invalid_argument("CharString::CharString() got a negative size.");
            resize(size, character, attributes);
        }
        CharString(Short size, const CharInfo& charInfo)
        {
            if (size < 0)
                throw std::invalid_argument("CharString::CharString() got a negative size.");
            resize(size, charInfo);
        }
        CharString(const std::wstring& string)
        {
            resize(string.length());
            for (Short i = 0; i < string.length(); ++i)
                inner.string[i] = CharInfo{ string[i], stdAttr };
        }
        CharString(const std::wstring& string, Attribute attributes)
        {
            resize(string.length());
            for (Short i = 0; i < string.length(); ++i)
                inner.string[i] = CharInfo{ string[i], attributes };
        }
        CharString(const Wchar* string)
        {
            Short length = 0;
            for (; string[length]; ++length);  // get the length
            resize(length);
            for (Short i = 0; i < length; ++i)
                inner.string[i] = CharInfo{ string[i], stdAttr };
        }
        CharString(const Wchar* string, Attribute attributes)
        {
            Short length = 0;
            for (; string[length]; ++length);
            resize(length);
            for (Short i = 0; i < length; ++i)
                inner.string[i] = CharInfo{ string[i], attributes };
        }
        CharString(const Wchar string[], Short length)
        {
            if (length < 0)
                throw std::invalid_argument("CharString::CharString() got a negative size.");
            resize(length);
            for (Short i = 0; i < length; ++i)
                inner.string[i] = CharInfo{ string[i], stdAttr };
        }
        CharString(const Wchar string[], Short length, Attribute attributes)
        {
            if (length < 0)
                throw std::invalid_argument("CharString::CharString() got a negative size.");
            resize(length);
            for (Short i = 0; i < length; ++i)
                inner.string[i] = CharInfo{ string[i], attributes };
        }

        CharInfo& at(Short i)
        {
            assert(within(i));
            if (!within(i))
                throw std::out_of_range("CharString::at() position out of bounds");
            return inner.string[i];
        }
        const CharInfo& at(Short i) const
        {
            assert(within(i));
            if (!within(i))
                throw std::out_of_range("CharString::at() position out of bounds");
            return inner.string[i];
        }

        CharInfo& operator[](Short i) noexcept
        {
            return inner.string[i];
        }
        const CharInfo& operator[](Short i) const noexcept
        {
            return inner.string[i];
        }

        void put(Short i, Wchar character, Attribute attributes)
        {
            put(i, CharInfo{ character, attributes });
        }
        void put(Short i, const CharInfo& charInfo)
        {
            assert(within(i));
            if (!within(i))
                throw std::out_of_range("CharString::put() position out of bounds");
            inner.string[i] = charInfo;
        }

        CharInfo* data() noexcept
        {
            return inner.string.data();
        }
        const CharInfo* data() const noexcept
        {
            return inner.string.data();
        }

        constexpr bool empty() const noexcept
        {
            return inner.size == 0;
        }

        Short size() const noexcept
        {
            return inner.size;
        }

        void resize(Short size, Wchar character, Attribute attributes)
        {
            resize(size, CharInfo{ character, attributes });
        }
        void resize(Short size, const CharInfo& charInfo)
        {
            if (size < 0)
                throw std::invalid_argument("CharString::resize() got a negative size.");
            inner.string.assign(size, charInfo);
            inner.size = size;
        }
        void resize(Short size)
        {
            resize(size, nullChar);
        }

        void fill(Wchar character, Attribute attributes) noexcept
        {
            fill(CharInfo{ character, attributes });
        }
        void fill(const CharInfo& charInfo) noexcept
        {
            std::fill(inner.string.begin(), inner.string.end(), charInfo);
        }

        void blank() noexcept
        {
            fill(nullChar);
        }

        constexpr bool within(Short i) const noexcept
        {
            return i >= 0 && i < inner.size;
        }

        void swap(CharString& other) noexcept
        {
            std::swap(inner.size, other.inner.size);
            inner.string.swap(other.inner.string);
        }
        friend void swap(CharString& a, CharString& b) noexcept
        {
            a.swap(b);
        }

    private:
        struct {
            Short baseStart, baseEnd;
            Short stackeeStart, stackeeEnd;
        } calculateIntersection(int base, int stackee, int offset) const noexcept
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
                 .baseStart = static_cast<Short>(left(base, offset)),
                 .baseEnd =   static_cast<Short>(right(base, stackee, offset)),
                 .stackeeStart = static_cast<Short>(left(stackee, -offset)),
                 .stackeeEnd =   static_cast<Short>(right(stackee, base, -offset)),
            };
        }

    public:
        CharString overlay(Short offset, const CharString& charString) const
        {
            auto isct = calculateIntersection(size(), charString.size(), offset);

            CharString string = *this;
            auto dest = string.data();
            auto source = charString.data();

            for (Short i = 0; isct.baseStart + i < isct.baseEnd; ++i)
                dest[isct.baseStart + i] = source[isct.stackeeStart + i];

            return string;
        }
        CharString overlay(const CharString& charString) const
        {
            return overlay(0, charString);
        }

        // review what happens when str.inlay(str)?
        void inlay(Short offset, const CharString& charString)
        {
            auto isct = calculateIntersection(size(), charString.size(), offset);

            auto dest = data();
            auto source = charString.data();

            for (Short i = 0; isct.baseStart + i < isct.baseEnd; ++i)
                dest[isct.baseStart + i] = source[isct.stackeeStart + i];
        }
        void inlay(const CharString& charString)
        {
            inlay(0, charString);
        }

        // right point is exclusive
        CharString slice(Short i1, Short i2) const
        {
            if (i1 < 0 || i1 > size() || i2 < 0 || i2 > size())
                throw std::invalid_argument("CharString::slice() got invalid coordinates");
            
            Short width = i2 - i1;
            if (width < 0)
                throw std::invalid_argument("CharString::slice() got invalid coordinates");

            CharString string(width);
            auto dest = string.data();
            auto source = data();

            for (Short i = 0; i < width; ++i)
                dest[i] = source[i + i1];

            return string;
        }

        void append(Wchar character, Attribute attributes)
        {
            inner.string.emplace_back(character, attributes);
            ++inner.size;
        }
        void append(const CharInfo& charInfo)
        {
            inner.string.push_back(charInfo);
            ++inner.size;
        }
        void append(const CharString& string)
        {
            inner.string.insert(inner.string.end(), string.inner.string.begin(), string.inner.string.end());
            inner.size += string.size();
        }

        void prepend(Wchar character, Attribute attributes)
        {
            inner.string.insert(inner.string.begin(), CharInfo{ character, attributes });
            ++inner.size;
        }
        void prepend(const CharInfo& charInfo)
        {
            inner.string.insert(inner.string.begin(), charInfo);
            ++inner.size;
        }
        void prepend(const CharString& string)
        {
            inner.string.insert(inner.string.begin(), string.inner.string.begin(), string.inner.string.end());
            inner.size += string.size();
        }

        CharString operator+(const CharString& other) const
        {
            CharString string = *this;
            string.append(other);
            return string;
        }
        CharString& operator+=(const CharString& other)
        {
            append(other);
            return *this;
        }

        bool operator==(const CharString& other) const
        {
            return inner.size == other.inner.size && inner.string == other.inner.string;
        }
        bool operator!=(const CharString& other) const
        {
            return inner.size != other.inner.size || inner.string != other.inner.string;
        }

        CharInfo* begin() noexcept
        {
            return data();
        }
        const CharInfo* begin() const noexcept
        {
            return data();
        }

        const CharInfo* cbegin() const noexcept
        {
            return data();
        }

        CharInfo* end() noexcept
        {
            return data() + size();
        }
        const CharInfo* end() const noexcept
        {
            return data() + size();
        }

        const CharInfo* cend() const noexcept
        {
            return data() + size();
        }

        // idea write iterators for characters and attributes separately
        // idea write methods for setting text/attributes
        // idea write method to convert to matrix
    };

    /*
     * Class `CharMatrix` represents rectangular field of cells containing characters and their attributes.
     */
    class CharMatrix
    {
    private:
        struct {
            Coord size;
            std::vector<CharInfo> matrix;
        } inner;

    public:
        static constexpr CharInfo nullChar{ 0, Attribute::No };

        CharMatrix()
        {
            resize(0, 0);
        }
        CharMatrix(Short x, Short y)
        {
            if (x < 0 || y < 0)
                throw std::invalid_argument("CharMatrix::CharMatrix() got a negative dimension.");
            resize(x, y);
        }
        CharMatrix(Short x, Short y, Wchar character, Attribute attributes)
        {
            if (x < 0 || y < 0)
                throw std::invalid_argument("CharMatrix::CharMatrix() got a negative dimension.");
            resize(x, y, character, attributes);
        }
        CharMatrix(const Coord& size)
        {
            if (size.x < 0 || size.y < 0)
                throw std::invalid_argument("CharMatrix::CharMatrix() got a negative dimension.");
            resize(size);
        }
        CharMatrix(const Coord& size, const CharInfo& character)
        {
            if (size.x < 0 || size.y < 0)
                throw std::invalid_argument("CharMatrix::CharMatrix() got a negative dimension.");
            resize(size, character);
        }

        CharInfo& at(Short x, Short y)
        {
            assert(within(x, y));
            if (!within(x, y))
                throw std::out_of_range("CharMatrix::at() position out of bounds");
            return inner.matrix[y * inner.size.x + x];
        }
        const CharInfo& at(Short x, Short y) const
        {
            assert(within(x, y));
            if (!within(x, y))
                throw std::out_of_range("CharMatrix::at() position out of bounds");
            return inner.matrix[y * inner.size.x + x];
        }
        CharInfo& at(const Coord& position)
        {
            return at(position.x, position.y);
        }
        const CharInfo& at(const Coord& position) const
        {
            return at(position.x, position.y);
        }

        CharInfo& operator[](const Coord& position) noexcept
        {
            return inner.matrix[position.y * inner.size.x + position.x];
        }
        const CharInfo& operator[](const Coord& position) const noexcept
        {
            return inner.matrix[position.y * inner.size.x + position.x];
        }

        void put(Short x, Short y, Wchar character, Attribute attributes)
        {
            put(x, y, CharInfo{ character, attributes });
        }
        void put(Short x, Short y, const CharInfo& charInfo)
        {
            assert(within(x, y));
            if (!within(x, y))
                throw std::out_of_range("CharMatrix::put() position out of bounds");
            inner.matrix[y * inner.size.x + x] = charInfo;
        }
        void put(const Coord& position, Wchar character, Attribute attributes)
        {
            put(position.x, position.y, character, attributes);
        }
        void put(const Coord& position, const CharInfo& charInfo)
        {
            put(position.x, position.y, charInfo.character, charInfo.attributes);
        }

        CharInfo* data() noexcept
        {
            return inner.matrix.data();
        }
        const CharInfo* data() const noexcept
        {
            return inner.matrix.data();
        }

        constexpr bool empty() const noexcept
        {
            return inner.size.x == 0 || inner.size.y == 0;
        }

        Coord size() const noexcept
        {
            return inner.size;
        }

        void resize(Short x, Short y, Wchar character, Attribute attributes)
        {
            resize(Coord{ x, y }, CharInfo{ character, attributes });
        }
        void resize(const Coord& size, const CharInfo& charInfo)
        {
            if (size.x < 0 || size.y < 0)
                throw std::invalid_argument("CharMatrix::resize() got a negative dimension.");
            inner.matrix.assign(size.x * size.y, charInfo);
            inner.size = size;
        }
        void resize(Short x, Short y)
        {
            resize(Coord{ x, y }, nullChar);
        }
        void resize(const Coord& size)
        {
            resize(size, nullChar);
        }

        void fill(Wchar character, Attribute attributes) noexcept
        {
            fill(CharInfo{ character, attributes });
        }
        void fill(const CharInfo& charInfo) noexcept
        {
            std::fill(inner.matrix.begin(), inner.matrix.end(), charInfo);
        }

        void blank() noexcept
        {
            fill(nullChar);
        }

        constexpr bool within(Short x, Short y) const noexcept
        {
            return x >= 0 && x < inner.size.x && y >= 0 && y < inner.size.y;
        }
        constexpr bool within(const Coord& position) const noexcept
        {
            return within(position.x, position.y);
        }

        void swap(CharMatrix& other) noexcept
        {
            std::swap(inner.size, other.inner.size);
            inner.matrix.swap(other.inner.matrix);
        }
        friend void swap(CharMatrix& a, CharMatrix& b) noexcept
        {
            a.swap(b);
        }

    private:
        struct {
            Short baseStartX, baseEndX;
            Short baseStartY, baseEndY;
            Short stackeeStartX, stackeeEndX;
            Short stackeeStartY, stackeeEndY;
        } calculateIntersection(int baseX, int baseY, int stackeeX, int stackeeY, int offsetX, int offsetY) const noexcept
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
                 .baseStartX = static_cast<Short>(left(baseX, offsetX)),
                 .baseEndX =   static_cast<Short>(right(baseX, stackeeX, offsetX)),
                 .baseStartY = static_cast<Short>(left(baseY, offsetY)),
                 .baseEndY =   static_cast<Short>(right(baseY, stackeeY, offsetY)),
                 .stackeeStartX = static_cast<Short>(left(stackeeX, -offsetX)),
                 .stackeeEndX =   static_cast<Short>(right(stackeeX, baseX, -offsetX)),
                 .stackeeStartY = static_cast<Short>(left(stackeeY, -offsetY)),
                 .stackeeEndY =   static_cast<Short>(right(stackeeY, baseY, -offsetY))
            };
        }

    public:
        CharMatrix overlay(Short x, Short y, const CharMatrix& charMatrix) const
        {
            Short baseX = size().x, baseY = size().y;
            Short stackeeX = charMatrix.size().x, stackeeY = charMatrix.size().y;
            
            auto isct = calculateIntersection(baseX, baseY, stackeeX, stackeeY, x, y);

            CharMatrix matrix = *this;
            auto dest = matrix.data();
            auto source = charMatrix.data();

            for (Short j = 0; isct.baseStartY + j < isct.baseEndY; ++j)
                for (Short i = 0; isct.baseStartX + i < isct.baseEndX; ++i)
                    dest[(j + isct.baseStartY) * baseX + i + isct.baseStartX] = source[(j + isct.stackeeStartY) * stackeeX + i + isct.stackeeStartX];

            return matrix;
        }
        CharMatrix overlay(const Coord& offset, const CharMatrix& charMatrix) const
        {
            return overlay(offset.x, offset.y, charMatrix);
        }
        CharMatrix overlay(const CharMatrix& charMatrix) const
        {
            return overlay(0, 0, charMatrix);
        }

        // review what happens when mat.inlay(mat)?
        void inlay(Short x, Short y, const CharMatrix& charMatrix)
        {
            Short baseX = size().x, baseY = size().y;
            Short stackeeX = charMatrix.size().x, stackeeY = charMatrix.size().y;

            auto isct = calculateIntersection(baseX, baseY, stackeeX, stackeeY, x, y);

            auto dest = data();
            auto source = charMatrix.data();

            for (Short j = 0; isct.baseStartY + j < isct.baseEndY; ++j)
                for (Short i = 0; isct.baseStartX + i < isct.baseEndX; ++i)
                    dest[(j + isct.baseStartY) * baseX + i + isct.baseStartX] = source[(j + isct.stackeeStartY) * stackeeX + i + isct.stackeeStartX];
        }
        void inlay(const Coord& offset, const CharMatrix& charMatrix)
        {
            inlay(offset.x, offset.y, charMatrix);
        }
        void inlay(const CharMatrix& charMatrix)
        {
            inlay(0, 0, charMatrix);
        }

        // bottom right point is exclusive
        CharMatrix slice(Short x1, Short y1, Short x2, Short y2) const
        {
            if (
                x1 < 0 || x1 > size().x || y1 < 0 || y1 > size().y ||
                x2 < 0 || x2 > size().x || y2 < 0 || y2 > size().y
            )
                throw std::invalid_argument("CharMatrix::slice() got invalid coordinates");
            
            Short width = x2 - x1, height = y2 - y1;
            if (width < 0 || height < 0)
                throw std::invalid_argument("CharMatrix::slice() got invalid coordinates");

            CharMatrix matrix(width, height);
            auto dest = matrix.data();
            auto source = data();

            for (Short j = 0; j < height; ++j)
                for (Short i = 0; i < width; ++i)
                    dest[j * width + i] = source[(j + y1) * size().x + i + x1];

            return matrix;
        }
        // bottom right point is exclusive
        CharMatrix slice(const Coord& topLeft, const Coord& bottomRight) const
        {
            return slice(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
        }
    };
}

#endif  // CINTER_INCLUDE_DEFINITIONS_CLASSES_HPP
