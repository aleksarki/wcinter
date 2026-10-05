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

#include "charcell.hpp"
#include "consts.hpp"
#include "enums.hpp"
#include "geometry.hpp"
#include "matrixmask.hpp"
#include "point.hpp"
#include "rect.hpp"
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
            Geometry size;
            std::vector<CharCell> matrix;
        } inner;

    public:
        template<typename T>
        class BasicRow;
        using Row = BasicRow<CharCell>;
        using ConstRow = BasicRow<const CharCell>;

        constexpr CharMatrix();
        constexpr CharMatrix(Short width, Short height);
        constexpr CharMatrix(Short width, Short height, Wchar character, Attribute attributes);
        constexpr CharMatrix(const Geometry& size);
        constexpr CharMatrix(const Geometry& size, const CharCell& cell);

        constexpr CharCell& at(Short x, Short y);
        constexpr const CharCell& at(Short x, Short y) const;
        constexpr CharCell& at(const Point& position);
        constexpr const CharCell& at(const Point& position) const;

        constexpr CharCell& operator[](const Point& position) noexcept;
        constexpr const CharCell& operator[](const Point& position) const noexcept;
        constexpr Row operator[](Short row) noexcept;
        constexpr ConstRow operator[](Short row) const noexcept;

        constexpr void put(Short x, Short y, Wchar character, Attribute attributes);
        constexpr void put(Short x, Short y, const CharCell& cell);
        constexpr void put(const Point& position, Wchar character, Attribute attributes);
        constexpr void put(const Point& position, const CharCell& cell);

        // todo implement string put

        constexpr CharCell* data() noexcept;
        constexpr const CharCell* data() const noexcept;

        constexpr CharCell* rowData(Short i) noexcept;
        constexpr const CharCell* rowData(Short i) const noexcept;

        constexpr bool empty() const noexcept;

        constexpr Geometry size() const noexcept;

        constexpr void resize(Short width, Short height, Wchar character, Attribute attributes);
        constexpr void resize(const Geometry& size, const CharCell& cell);
        constexpr void resize(Short width, Short height);
        constexpr void resize(const Geometry& size);

        constexpr void fill(Wchar character, Attribute attributes) noexcept;
        constexpr void fill(const CharCell& cell) noexcept;

        constexpr void blank() noexcept;

        constexpr bool within(Short x, Short y) const noexcept;
        constexpr bool within(const Point& position) const noexcept;

        constexpr void swap(CharMatrix& other) noexcept;
        friend constexpr void swap(CharMatrix& a, CharMatrix& b) noexcept;

        constexpr CharMatrix overlay(Short x, Short y, const CharMatrix& charMatrix) const;
        constexpr CharMatrix overlay(const Point& offset, const CharMatrix& charMatrix) const;
        constexpr CharMatrix overlay(const CharMatrix& charMatrix) const;

        // review what happens when mat.inlay(mat)?
        constexpr void inlay(Short x, Short y, const CharMatrix& charMatrix);
        constexpr void inlay(const Point& offset, const CharMatrix& charMatrix);
        constexpr void inlay(const CharMatrix& charMatrix);

        // bottom right point is exclusive
        constexpr CharMatrix slice(Short x1, Short y1, Short x2, Short y2) const;
        // bottom right point is exclusive
        constexpr CharMatrix slice(const Point& topLeft, const Point& bottomRight) const;
        // bottom right point is exclusive
        constexpr CharMatrix slice(const Rect& rect) const;

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
inline constexpr wci::CharMatrix::CharMatrix(wci::Short width, wci::Short height)
{
    if (width < 0 || height < 0)
        throw std::invalid_argument("CharMatrix::CharMatrix() got a negative dimension.");
    resize(width, height);
}
inline constexpr wci::CharMatrix::CharMatrix(wci::Short width, wci::Short height, wci::Wchar character, wci::Attribute attributes)
{
    if (width < 0 || height < 0)
        throw std::invalid_argument("CharMatrix::CharMatrix() got a negative dimension.");
    resize(width, height, character, attributes);
}
inline constexpr wci::CharMatrix::CharMatrix(const wci::Geometry& size)
{
    if (size.width < 0 || size.height < 0)
        throw std::invalid_argument("CharMatrix::CharMatrix() got a negative dimension.");
    resize(size);
}
inline constexpr wci::CharMatrix::CharMatrix(const wci::Geometry& size, const wci::CharCell& cell)
{
    if (size.width < 0 || size.height < 0)
        throw std::invalid_argument("CharMatrix::CharMatrix() got a negative dimension.");
    resize(size, cell);
}

inline constexpr wci::CharCell& wci::CharMatrix::at(wci::Short x, wci::Short y)
{
    if (!within(x, y))
        throw std::out_of_range("CharMatrix::at() position out of bounds");
    return inner.matrix[y * size().width + x];
}
inline constexpr const wci::CharCell& wci::CharMatrix::at(wci::Short x, wci::Short y) const
{
    if (!within(x, y))
        throw std::out_of_range("CharMatrix::at() position out of bounds");
    return inner.matrix[y * size().width + x];
}
inline constexpr wci::CharCell& wci::CharMatrix::at(const wci::Point& position)
{
    return at(position.x, position.y);
}
inline constexpr const wci::CharCell& wci::CharMatrix::at(const wci::Point& position) const
{
    return at(position.x, position.y);
}

inline constexpr wci::CharCell& wci::CharMatrix::operator[](const wci::Point& position) noexcept
{
    return inner.matrix[position.y * size().width + position.x];
}
inline constexpr const wci::CharCell& wci::CharMatrix::operator[](const wci::Point& position) const noexcept
{
    return inner.matrix[position.y * size().width + position.x];
}

inline constexpr wci::CharMatrix::Row wci::CharMatrix::operator[](wci::Short i) noexcept
{
    return wci::CharMatrix::Row(data() + i * size().width, size().width);
}

inline constexpr wci::CharMatrix::ConstRow wci::CharMatrix::operator[](wci::Short i) const noexcept
{
    return wci::CharMatrix::ConstRow(data() + i * size().width, size().width);
}

inline constexpr void wci::CharMatrix::put(wci::Short x, wci::Short y, wci::Wchar character, wci::Attribute attributes)
{
    put(x, y, wci::CharCell{ character, attributes });
}
inline constexpr void wci::CharMatrix::put(wci::Short x, wci::Short y, const wci::CharCell& cell)
{
    if (!within(x, y))
        throw std::out_of_range("CharMatrix::put() position out of bounds");
    inner.matrix[y * size().width + x] = cell;
}
inline constexpr void wci::CharMatrix::put(const wci::Point& position, wci::Wchar character, wci::Attribute attributes)
{
    put(position.x, position.y, character, attributes);
}
inline constexpr void wci::CharMatrix::put(const wci::Point& position, const wci::CharCell& cell)
{
    put(position.x, position.y, cell.character, cell.attributes);
}

inline constexpr wci::CharCell* wci::CharMatrix::data() noexcept
{
    return inner.matrix.data();
}
inline constexpr const wci::CharCell* wci::CharMatrix::data() const noexcept
{
    return inner.matrix.data();
}

inline constexpr wci::CharCell* wci::CharMatrix::rowData(wci::Short i) noexcept
{
    return data() + i * size().width;
}
inline constexpr const wci::CharCell* wci::CharMatrix::rowData(wci::Short i) const noexcept
{
    return data() + i * size().width;
}

inline constexpr bool wci::CharMatrix::empty() const noexcept
{
    return size().empty();
}

inline constexpr wci::Geometry wci::CharMatrix::size() const noexcept
{
    return inner.size;
}

inline constexpr void wci::CharMatrix::resize(wci::Short width, wci::Short height, wci::Wchar character, wci::Attribute attributes)
{
    resize(wci::Geometry{ width, height }, wci::CharCell{ character, attributes });
}
inline constexpr void wci::CharMatrix::resize(const wci::Geometry& size, const wci::CharCell& cell)
{
    if (size.width < 0 || size.height < 0)
        throw std::invalid_argument("CharMatrix::resize() got a negative dimension.");
    inner.matrix.assign(size.width * size.height, cell);
    inner.size = size;
}
inline constexpr void wci::CharMatrix::resize(wci::Short width, wci::Short height)
{
    resize(wci::Geometry{ width, height }, wci::nullch);
}
inline constexpr void wci::CharMatrix::resize(const wci::Geometry& size)
{
    resize(size, wci::nullch);
}

inline constexpr void wci::CharMatrix::fill(wci::Wchar character, wci::Attribute attributes) noexcept
{
    fill(wci::CharCell{ character, attributes });
}
// todo use CharMatrix:: begin end
inline constexpr void wci::CharMatrix::fill(const wci::CharCell& cell) noexcept
{
    std::fill(inner.matrix.begin(), inner.matrix.end(), cell);
}

inline constexpr void wci::CharMatrix::blank() noexcept
{
    fill(wci::nullch);
}

inline constexpr bool wci::CharMatrix::within(wci::Short x, wci::Short y) const noexcept
{
    return x >= 0 && x < size().width && y >= 0 && y < size().height;
}
inline constexpr bool wci::CharMatrix::within(const wci::Point& position) const noexcept
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

inline constexpr wci::CharMatrix wci::CharMatrix::overlay(wci::Short x, wci::Short y, const wci::CharMatrix& other) const
{
    return overlay(wci::Point{ x, y }, other);
}
inline constexpr wci::CharMatrix wci::CharMatrix::overlay(const wci::Point& offset, const wci::CharMatrix& other) const
{
    auto base = size().toRect();
    auto stackee = other.size().toRect() + offset;
    auto overlap = base.intersect(stackee);
    if (!overlap.valid())  // no overlap
        return *this;
    
    wci::CharMatrix matrix = *this;
    for (wci::Short i = overlap.top; i < overlap.bottom; ++i)
        for (wci::Short j = overlap.left; j < overlap.right; ++j)
            matrix.rowData(i)[j] = other.rowData(i - offset.y)[j - offset.x];

    return matrix;
}
inline constexpr wci::CharMatrix wci::CharMatrix::overlay(const wci::CharMatrix& other) const
{
    return overlay(wci::Point{ 0, 0 }, other);
}

inline constexpr void wci::CharMatrix::inlay(wci::Short x, wci::Short y, const wci::CharMatrix& other)
{
    inlay(wci::Point{ x, y }, other);
}
inline constexpr void wci::CharMatrix::inlay(const wci::Point &offset, const wci::CharMatrix& other)
{
    auto base = size().toRect();
    auto stackee = other.size().toRect() + offset;
    auto overlap = base.intersect(stackee);
    if (!overlap.valid())  // no overlap
        return;

    for (wci::Short i = overlap.top; i < overlap.bottom; ++i)
        for (wci::Short j = overlap.left; j < overlap.right; ++j)
            rowData(i)[j] = other.rowData(i - offset.y)[j - offset.x];
}
inline constexpr void wci::CharMatrix::inlay(const wci::CharMatrix& other)
{
    inlay(wci::Point{ 0, 0 }, other);
}

inline constexpr wci::CharMatrix wci::CharMatrix::slice(wci::Short x1, wci::Short y1, wci::Short x2, wci::Short y2) const
{
    return slice(wci::Rect{ x1, y1, x2, y2 });
}
inline constexpr wci::CharMatrix wci::CharMatrix::slice(const wci::Point& topLeft, const wci::Point& bottomRight) const
{
    return slice(wci::Rect{ topLeft.x, topLeft.y, bottomRight.x, bottomRight.y });
}
inline constexpr wci::CharMatrix wci::CharMatrix::slice(const wci::Rect& rect) const
{
    if (!size().within(rect))
        throw std::out_of_range("CharMatrix::slice() got invalid coordinates");

    if (!rect.valid())
        throw std::invalid_argument("CharMatrix::slice() got invalid coordinates");

    wci::CharMatrix matrix(rect.size());
    for (wci::Short i = 0; i < rect.size().height; ++i)
        for (wci::Short j = 0; j < rect.size().width; ++j)
            matrix.rowData(i)[j] = rowData(i + rect.top)[j + rect.left];

    return matrix;
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

    for (wci::Short i = 0; i < size().height; ++i)
    {
        if (std::equal(rowData(i), rowData(i) + size().width, other.rowData(i)))  // whole row has no difference
            continue;

        for (wci::Short j = 0; j < size().width; ++j)
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
