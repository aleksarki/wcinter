#pragma once
#ifndef CINTER_INCLUDE_DEFINITIONS_CLASSES_HPP
#define CINTER_INCLUDE_DEFINITIONS_CLASSES_HPP

#include <algorithm>
#include <cassert>
#include <stdexcept>
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

        CharMatrix(Short x, Short y) noexcept
        {
            resize(x, y);
        }
        CharMatrix(const Coord& size) noexcept
        {
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

        CharInfo& operator[](const Coord& position)
        {
            return at(position);
        }
        const CharInfo& operator[](const Coord& position) const
        {
            return at(position);
        }

        void put(Short x, Short y, wchar_t character, Attribute attributes)
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
        void put(const Coord& position, wchar_t character, Attribute attributes)
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

        const Coord size() const noexcept
        {
            return inner.size;
        }

        void resize(Short x, Short y)
        {
            resize(Coord{ x, y }, nullChar);
        }
        void resize(const Coord& size)
        {
            resize(size, nullChar);
        }
        void resize(Short x, Short y, wchar_t character, Attribute attributes)
        {
            resize(Coord{ x, y }, CharInfo{ character, attributes });
        }
        void resize(const Coord& size, const CharInfo& charInfo)
        {
            inner.size = Coord{ size.x, size.y };
            inner.matrix.resize(inner.size.x * inner.size.y);
            fill(charInfo);
        }

        void fill(wchar_t character, Attribute attributes)
        {
            fill(CharInfo{ character, attributes });
        }
        void fill(const CharInfo& charInfo)
        {
            std::fill(inner.matrix.begin(), inner.matrix.end(), charInfo);
        }

        void blank()
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
    };
}

#endif  // CINTER_INCLUDE_DEFINITIONS_CLASSES_HPP
