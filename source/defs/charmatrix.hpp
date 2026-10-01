#pragma once
#ifndef WCI_SOURCE_DEFS_CHARMATRIX_HPP
#define WCI_SOURCE_DEFS_CHARMATRIX_HPP

#include <algorithm>
#include <cassert>
#include <span>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "consts.hpp"
#include "enums.hpp"
#include "matrixmask.hpp"
#include "structs.hpp"
#include "types.hpp"

namespace wci
{
    class CharString;
    class CharMatrix;

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
        template<typename T>
        class BasicRow;
        using Row = BasicRow<CharInfo>;
        using ConstRow = BasicRow<const CharInfo>;

        constexpr CharMatrix();
        constexpr CharMatrix(Short x, Short y);
        constexpr CharMatrix(Short x, Short y, Wchar character, Attribute attributes);
        constexpr CharMatrix(const Coord& size);
        constexpr CharMatrix(const Coord& size, const CharInfo& character);

        constexpr CharInfo& at(Short x, Short y);
        constexpr const CharInfo& at(Short x, Short y) const;
        constexpr CharInfo& at(const Coord& position);
        constexpr const CharInfo& at(const Coord& position) const;

        constexpr CharInfo& operator[](const Coord& position) noexcept;
        constexpr const CharInfo& operator[](const Coord& position) const noexcept;
        constexpr Row operator[](Short row) noexcept;
        constexpr ConstRow operator[](Short row) const noexcept;

        constexpr void put(Short x, Short y, Wchar character, Attribute attributes);
        constexpr void put(Short x, Short y, const CharInfo& charInfo);
        constexpr void put(const Coord& position, Wchar character, Attribute attributes);
        constexpr void put(const Coord& position, const CharInfo& charInfo);

        // todo implement string put

        constexpr CharInfo* data() noexcept;
        constexpr const CharInfo* data() const noexcept;

        constexpr CharInfo* rowData(Short i) noexcept;
        constexpr const CharInfo* rowData(Short i) const noexcept;

        constexpr bool empty() const noexcept;

        constexpr Coord size() const noexcept;

        constexpr void resize(Short x, Short y, Wchar character, Attribute attributes);
        constexpr void resize(const Coord& size, const CharInfo& charInfo);
        constexpr void resize(Short x, Short y);
        constexpr void resize(const Coord& size);

        constexpr void fill(Wchar character, Attribute attributes) noexcept;
        constexpr void fill(const CharInfo& charInfo) noexcept;

        constexpr void blank() noexcept;

        constexpr bool within(Short x, Short y) const noexcept;
        constexpr bool within(const Coord& position) const noexcept;

        constexpr void swap(CharMatrix& other) noexcept;
        friend constexpr void swap(CharMatrix& a, CharMatrix& b) noexcept;

    private:
        struct Intersection
        {
            Short baseStartX, baseEndX;
            Short baseStartY, baseEndY;
            Short stackeeStartX, stackeeEndX;
            Short stackeeStartY, stackeeEndY;
        };

        constexpr Intersection calculateIntersection(int baseX, int baseY, int stackeeX, int stackeeY, int offsetX, int offsetY) const noexcept;

    public:
        constexpr CharMatrix overlay(Short x, Short y, const CharMatrix& charMatrix) const;
        constexpr CharMatrix overlay(const Coord& offset, const CharMatrix& charMatrix) const;
        constexpr CharMatrix overlay(const CharMatrix& charMatrix) const;

        // review what happens when mat.inlay(mat)?
        constexpr void inlay(Short x, Short y, const CharMatrix& charMatrix);
        constexpr void inlay(const Coord& offset, const CharMatrix& charMatrix);
        constexpr void inlay(const CharMatrix& charMatrix);

        // bottom right point is exclusive
        constexpr CharMatrix slice(Short x1, Short y1, Short x2, Short y2) const;
        // bottom right point is exclusive
        constexpr CharMatrix slice(const Coord& topLeft, const Coord& bottomRight) const;

        constexpr MatrixMask difference(const CharMatrix& other) const;

        constexpr CharMatrix toMatrix() const noexcept;

        // idea write begin/end methods
        // idea implement row iterators

        template<typename T>
        class BasicRow
        {
        private:
            std::span<T> row;

        public:
            constexpr BasicRow(T* data, Short size);

            constexpr Short size() const noexcept;

            constexpr T& operator[](Short j) const noexcept;

            constexpr T* data() const noexcept;

            constexpr T* begin() const noexcept;

            constexpr const T* cbegin() const noexcept;

            constexpr T* end() const noexcept;

            constexpr const T* cend() const noexcept;
        };
    };
}

inline constexpr wci::CharMatrix::CharMatrix()
{
    inner.matrix.clear();
    resize(0, 0);
}
inline constexpr wci::CharMatrix::CharMatrix(wci::Short x, wci::Short y)
{
    if (x < 0 || y < 0)
        throw std::invalid_argument("CharMatrix::CharMatrix() got a negative dimension.");
    resize(x, y);
}
inline constexpr wci::CharMatrix::CharMatrix(wci::Short x, wci::Short y, wci::Wchar character, wci::Attribute attributes)
{
    if (x < 0 || y < 0)
        throw std::invalid_argument("CharMatrix::CharMatrix() got a negative dimension.");
    resize(x, y, character, attributes);
}
inline constexpr wci::CharMatrix::CharMatrix(const wci::Coord& size)
{
    if (size.x < 0 || size.y < 0)
        throw std::invalid_argument("CharMatrix::CharMatrix() got a negative dimension.");
    resize(size);
}
inline constexpr wci::CharMatrix::CharMatrix(const wci::Coord& size, const wci::CharInfo& character)
{
    if (size.x < 0 || size.y < 0)
        throw std::invalid_argument("CharMatrix::CharMatrix() got a negative dimension.");
    resize(size, character);
}

inline constexpr wci::CharInfo& wci::CharMatrix::at(wci::Short x, wci::Short y)
{
    if (!within(x, y))
        throw std::out_of_range("CharMatrix::at() position out of bounds");
    return inner.matrix[y * inner.size.x + x];
}
inline constexpr const wci::CharInfo& wci::CharMatrix::at(wci::Short x, wci::Short y) const
{
    if (!within(x, y))
        throw std::out_of_range("CharMatrix::at() position out of bounds");
    return inner.matrix[y * inner.size.x + x];
}
inline constexpr wci::CharInfo& wci::CharMatrix::at(const wci::Coord& position)
{
    return at(position.x, position.y);
}
inline constexpr const wci::CharInfo& wci::CharMatrix::at(const wci::Coord& position) const
{
    return at(position.x, position.y);
}

inline constexpr wci::CharInfo& wci::CharMatrix::operator[](const wci::Coord& position) noexcept
{
    return inner.matrix[position.y * inner.size.x + position.x];
}
inline constexpr const wci::CharInfo& wci::CharMatrix::operator[](const wci::Coord& position) const noexcept
{
    return inner.matrix[position.y * inner.size.x + position.x];
}

inline constexpr wci::CharMatrix::Row wci::CharMatrix::operator[](wci::Short i) noexcept
{
    return wci::CharMatrix::Row(data() + i * size().x, size().x);
}

inline constexpr wci::CharMatrix::ConstRow wci::CharMatrix::operator[](wci::Short i) const noexcept
{
    return wci::CharMatrix::ConstRow(data() + i * size().x, size().x);
}

inline constexpr void wci::CharMatrix::put(wci::Short x, wci::Short y, wci::Wchar character, wci::Attribute attributes)
{
    put(x, y, wci::CharInfo{ character, attributes });
}
inline constexpr void wci::CharMatrix::put(wci::Short x, wci::Short y, const wci::CharInfo& charInfo)
{
    if (!within(x, y))
        throw std::out_of_range("CharMatrix::put() position out of bounds");
    inner.matrix[y * inner.size.x + x] = charInfo;
}
inline constexpr void wci::CharMatrix::put(const wci::Coord& position, wci::Wchar character, wci::Attribute attributes)
{
    put(position.x, position.y, character, attributes);
}
inline constexpr void wci::CharMatrix::put(const wci::Coord& position, const wci::CharInfo& charInfo)
{
    put(position.x, position.y, charInfo.character, charInfo.attributes);
}

inline constexpr wci::CharInfo* wci::CharMatrix::data() noexcept
{
    return inner.matrix.data();
}
inline constexpr const wci::CharInfo* wci::CharMatrix::data() const noexcept
{
    return inner.matrix.data();
}

inline constexpr wci::CharInfo* wci::CharMatrix::rowData(wci::Short i) noexcept
{
    return data() + i * size().x;
}
inline constexpr const wci::CharInfo* wci::CharMatrix::rowData(wci::Short i) const noexcept
{
    return data() + i * size().x;
}

inline constexpr bool wci::CharMatrix::empty() const noexcept
{
    return size().x == 0 || size().y == 0;
}

inline constexpr wci::Coord wci::CharMatrix::size() const noexcept
{
    return inner.size;
}

inline constexpr void wci::CharMatrix::resize(wci::Short x, wci::Short y, wci::Wchar character, wci::Attribute attributes)
{
    resize(wci::Coord{ x, y }, wci::CharInfo{ character, attributes });
}
inline constexpr void wci::CharMatrix::resize(const wci::Coord& size, const wci::CharInfo& charInfo)
{
    if (size.x < 0 || size.y < 0)
        throw std::invalid_argument("CharMatrix::resize() got a negative dimension.");
    inner.matrix.assign(size.x * size.y, charInfo);
    inner.size = size;
}
inline constexpr void wci::CharMatrix::resize(wci::Short x, wci::Short y)
{
    resize(wci::Coord{ x, y }, wci::nullch);
}
inline constexpr void wci::CharMatrix::resize(const wci::Coord& size)
{
    resize(size, wci::nullch);
}

inline constexpr void wci::CharMatrix::fill(wci::Wchar character, wci::Attribute attributes) noexcept
{
    fill(wci::CharInfo{ character, attributes });
}
// todo use CharMatrix:: begin end
inline constexpr void wci::CharMatrix::fill(const wci::CharInfo& charInfo) noexcept
{
    std::fill(inner.matrix.begin(), inner.matrix.end(), charInfo);
}

inline constexpr void wci::CharMatrix::blank() noexcept
{
    fill(wci::nullch);
}

inline constexpr bool wci::CharMatrix::within(wci::Short x, wci::Short y) const noexcept
{
    return x >= 0 && x < inner.size.x && y >= 0 && y < inner.size.y;
}
inline constexpr bool wci::CharMatrix::within(const wci::Coord& position) const noexcept
{
    return within(position.x, position.y);
}

inline constexpr void wci::CharMatrix::swap(wci::CharMatrix& other) noexcept
{
    std::swap(inner.size, other.inner.size);
    inner.matrix.swap(other.inner.matrix);
}
inline constexpr void wci::swap(wci::CharMatrix& a, wci::CharMatrix& b) noexcept
{
    a.swap(b);
}

inline constexpr wci::CharMatrix::Intersection wci::CharMatrix::calculateIntersection(
    int baseX, int baseY, int stackeeX, int stackeeY, int offsetX, int offsetY
) const noexcept
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
         .baseStartX =    static_cast<Short>(left (baseX, offsetX)),
         .baseEndX =      static_cast<Short>(right(baseX, stackeeX, offsetX)),
         .baseStartY =    static_cast<Short>(left (baseY, offsetY)),
         .baseEndY =      static_cast<Short>(right(baseY, stackeeY, offsetY)),
         .stackeeStartX = static_cast<Short>(left (stackeeX, -offsetX)),
         .stackeeEndX =   static_cast<Short>(right(stackeeX, baseX, -offsetX)),
         .stackeeStartY = static_cast<Short>(left (stackeeY, -offsetY)),
         .stackeeEndY =   static_cast<Short>(right(stackeeY, baseY, -offsetY))
    };
}

inline constexpr wci::CharMatrix wci::CharMatrix::overlay(wci::Short x, wci::Short y, const wci::CharMatrix& charMatrix) const
{
    wci::Short baseX = size().x, baseY = size().y;
    wci::Short stackeeX = charMatrix.size().x, stackeeY = charMatrix.size().y;

    auto isct = calculateIntersection(baseX, baseY, stackeeX, stackeeY, x, y);

    wci::CharMatrix matrix = *this;
    auto dest = matrix.data();
    auto source = charMatrix.data();

    for (wci::Short j = 0; isct.baseStartY + j < isct.baseEndY; ++j)
        for (wci::Short i = 0; isct.baseStartX + i < isct.baseEndX; ++i)
            dest[(j + isct.baseStartY) * baseX + i + isct.baseStartX] = source[(j + isct.stackeeStartY) * stackeeX + i + isct.stackeeStartX];

    return matrix;
}
inline constexpr wci::CharMatrix wci::CharMatrix::overlay(const wci::Coord& offset, const wci::CharMatrix& charMatrix) const
{
    return overlay(offset.x, offset.y, charMatrix);
}
inline constexpr wci::CharMatrix wci::CharMatrix::overlay(const wci::CharMatrix& charMatrix) const
{
    return overlay(0, 0, charMatrix);
}

inline constexpr void wci::CharMatrix::inlay(wci::Short x, wci::Short y, const wci::CharMatrix& charMatrix)
{
    wci::Short baseX = size().x, baseY = size().y;
    wci::Short stackeeX = charMatrix.size().x, stackeeY = charMatrix.size().y;

    auto isct = calculateIntersection(baseX, baseY, stackeeX, stackeeY, x, y);

    auto dest = data();
    auto source = charMatrix.data();

    for (wci::Short j = 0; isct.baseStartY + j < isct.baseEndY; ++j)
        for (wci::Short i = 0; isct.baseStartX + i < isct.baseEndX; ++i)
            dest[(j + isct.baseStartY) * baseX + i + isct.baseStartX] = source[(j + isct.stackeeStartY) * stackeeX + i + isct.stackeeStartX];
}
inline constexpr void wci::CharMatrix::inlay(const wci::Coord &offset, const wci::CharMatrix& charMatrix)
{
    inlay(offset.x, offset.y, charMatrix);
}
inline constexpr void wci::CharMatrix::inlay(const wci::CharMatrix& charMatrix)
{
    inlay(0, 0, charMatrix);
}

inline constexpr wci::CharMatrix wci::CharMatrix::slice(wci::Short x1, wci::Short y1, wci::Short x2, wci::Short y2) const
{
    if (
        x1 < 0 || x1 > size().x || y1 < 0 || y1 > size().y ||
        x2 < 0 || x2 > size().x || y2 < 0 || y2 > size().y
    )
        throw std::invalid_argument("CharMatrix::slice() got invalid coordinates");

    wci::Short width = x2 - x1, height = y2 - y1;
    if (width < 0 || height < 0)
        throw std::invalid_argument("CharMatrix::slice() got invalid coordinates");

    wci::CharMatrix matrix(width, height);
    auto dest = matrix.data();
    auto source = data();

    for (wci::Short j = 0; j < height; ++j)
        for (wci::Short i = 0; i < width; ++i)
            dest[j * width + i] = source[(j + y1) * size().x + i + x1];

    return matrix;
}
inline constexpr wci::CharMatrix wci::CharMatrix::slice(const wci::Coord& topLeft, const wci::Coord& bottomRight) const
{
    return slice(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}

inline constexpr wci::MatrixMask wci::CharMatrix::difference(const wci::CharMatrix& other) const
{
    if (other.size() != size())  // we allow for sizes to not be equal
    {
        auto matrix = overlay(other);
        return difference(matrix);
    }

    wci::MatrixMask mask(size());
    if (&other == this)
        return mask;

    for (wci::Short i = 0; i < size().y; ++i)
    {
        if (std::equal(rowData(i), rowData(i) + size().x, other.rowData(i)))  // whole row has no difference
            continue;

        for (wci::Short j = 0; j < size().x; ++j)
            if (rowData(i)[j] != other.rowData(i)[j])
                mask.set(j, i);
    }

    return mask;
}

inline constexpr wci::CharMatrix wci::CharMatrix::toMatrix() const noexcept
{
    return *this;
}

template<typename T>
inline constexpr wci::CharMatrix::BasicRow<T>::BasicRow(T* data, wci::Short size) : row(data, size)
{}

template<typename T>
inline constexpr wci::Short wci::CharMatrix::BasicRow<T>::size() const noexcept
{
    return static_cast<wci::Short>(row.size());
}

template<typename T>
inline constexpr T& wci::CharMatrix::BasicRow<T>::operator[](wci::Short j) const noexcept
{
    return row[j];
}

template<typename T>
inline constexpr T* wci::CharMatrix::BasicRow<T>::data() const noexcept
{
    return row.data();
}

template<typename T>
inline constexpr T* wci::CharMatrix::BasicRow<T>::begin() const noexcept
{
    return row.data();
}

template<typename T>
inline constexpr const T* wci::CharMatrix::BasicRow<T>::cbegin() const noexcept
{
    return row.data();
}

template<typename T>
inline constexpr T* wci::CharMatrix::BasicRow<T>::end() const noexcept
{
    return row.data() + row.size();
}

template<typename T>
inline constexpr const T* wci::CharMatrix::BasicRow<T>::cend() const noexcept
{
    return row.data() + row.size();
}

#endif  // WCI_SOURCE_DEFS_CHARMATRIX_HPP
