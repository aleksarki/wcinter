#pragma once
#ifndef CINTER_INCLUDE_STRUCTS_HPP
#define CINTER_INCLUDE_STRUCTS_HPP

#include "definitions/enums.hpp"
#include "definitions/structs.hpp"
#include "definitions/types.hpp"

#include <vector>

namespace wci
{
    class CharMatrix
    {
    private:
        std::vector<CharInfo> mat;
        Coord siz;
    
    public:
        CharMatrix(Short x, Short y) : siz{ x, y }
        {
            mat.resize(siz.x * siz.y);
        }
        CharMatrix(Coord size) : siz(size)
        {
            mat.resize(size.x * size.y);
        }

        CharInfo& at(Short x, Short y) noexcept
        {
            return mat[y * siz.x + x];
        }
        const CharInfo& at(Short x, Short y) const noexcept
        {
            return mat[y * siz.x + x];
        }
        CharInfo& at(const Coord& position) noexcept
        {
            return mat[position.y * siz.x + position.x];
        }
        const CharInfo& at(const Coord& position) const noexcept
        {
            return mat[position.y * siz.x + position.x];
        }

        void put(Short x, Short y, CharInfo charInfo)
        {
            mat[y * siz.x + x] = charInfo;
        }
        void put(Short x, Short y, wchar_t character, Attribute attributes)
        {
            mat[y * siz.x + x] = CharInfo{ character, attributes };
        }
        void put(const Coord& position, CharInfo charInfo)
        {
            mat[position.y * siz.x + position.x] = charInfo;
        }
        void put(const Coord& position, wchar_t character, Attribute attributes)
        {
            mat[position.y * siz.x + position.x] = CharInfo{ character, attributes };
        }
    
        CharInfo* data() noexcept
        {
            return mat.data();
        }
        const CharInfo* data() const noexcept
        {
            return mat.data();
        }
    
        Coord& size() noexcept
        {
            return siz;
        }
        const Coord& size() const noexcept
        {
            return siz;
        }
    };
}

#endif  // CINTER_INCLUDE_STRUCTS_HPP
