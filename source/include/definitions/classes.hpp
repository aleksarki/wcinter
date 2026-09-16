#pragma once
#ifndef CINTER_INCLUDE_DEFINITIONS_CLASSES_HPP
#define CINTER_INCLUDE_DEFINITIONS_CLASSES_HPP

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <utility>
#include <vector>

#include "enums.hpp"
#include "structs.hpp"
#include "types.hpp"

namespace wci
{
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

        CharMatrix(Short x, Short y)
        {
            if (x < 0 || y < 0)
                throw std::invalid_argument("CharMatrix::CharMatrix() got a negative dimension.");
            resize(x, y);
        }
        CharMatrix(const Coord& size)
        {
            if (size.x < 0 || size.y < 0)
                throw std::invalid_argument("CharMatrix::CharMatrix() got a negative dimension.");
            resize(size);
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

        bool empty() const noexcept
        {
            return inner.size.x == 0 || inner.size.y == 0;
        }

        const Coord& size() const noexcept
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

        bool within(Short x, Short y) const noexcept
        {
            return x >= 0 && x < inner.size.x && y >= 0 && y < inner.size.y;
        }
        bool within(const Coord& position) const noexcept
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
        CharMatrix overlay(Short x, Short y, const CharMatrix& other) const
        {
            Short baseX = size().x, baseY = size().y;
            Short stackeeX = other.size().x, stackeeY = other.size().y;
            
            auto isct = calculateIntersection(baseX, baseY, stackeeX, stackeeY, x, y);

            CharMatrix matrix = *this;
            auto dest = matrix.data();
            auto source = other.data();

            for (Short j = 0; isct.baseStartY + j < isct.baseEndY; ++j)
                for (Short i = 0; isct.baseStartX + i < isct.baseEndX; ++i)
                    dest[(j + isct.baseStartY) * baseX + i + isct.baseStartX] = source[(j + isct.stackeeStartY) * stackeeX + i + isct.stackeeStartX];

            return matrix;
        }
        CharMatrix overlay(const Coord& offset, const CharMatrix& other) const
        {
            return overlay(offset.x, offset.y, other);
        }
        CharMatrix overlay(const CharMatrix& other) const
        {
            return overlay(0, 0, other);
        }

        // review what happens when mat.inlay(mat)?
        void inlay(Short x, Short y, const CharMatrix& other)
        {
            Short baseX = size().x, baseY = size().y;
            Short stackeeX = other.size().x, stackeeY = other.size().y;

            auto isct = calculateIntersection(baseX, baseY, stackeeX, stackeeY, x, y);

            auto dest = data();
            auto source = other.data();

            for (Short j = 0; isct.baseStartY + j < isct.baseEndY; ++j)
                for (Short i = 0; isct.baseStartX + i < isct.baseEndX; ++i)
                    dest[(j + isct.baseStartY) * baseX + i + isct.baseStartX] = source[(j + isct.stackeeStartY) * stackeeX + i + isct.stackeeStartX];
        }
        void inlay(const Coord& offset, const CharMatrix& other)
        {
            inlay(offset.x, offset.y, other);
        }
        void inlay(const CharMatrix& other)
        {
            inlay(0, 0, other);
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
        CharMatrix slice(const Coord& topLeft, const Coord& bottomRight) const
        {
            return slice(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
        }
    };
}

#endif  // CINTER_INCLUDE_DEFINITIONS_CLASSES_HPP
