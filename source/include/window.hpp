#pragma once
#ifndef CINTER_INCLUDE_WINDOW_HPP
#define CINTER_INCLUDE_WINDOW_HPP

#include <memory>
#include "definitions.hpp"
#include "console.hpp"

namespace wci {

    class Window
    {
    public:
        Window();
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        Console& console() noexcept;
        const Console& console() const noexcept;

        CharMatrix& matrix() noexcept;
        const CharMatrix& matrix() const noexcept;

        void render();

        void resize();  // todo on buffer resize; recreate underlying matrix; use Matrix::merge to get the overlap

        void printChar(wchar_t character);
        void printString(const wchar_t* string);

        void putChar(Short x, Short y, wchar_t character);
        void putChar(Short x, Short y, Wchar character, Attribute attributes);
        void putChar(const Coord& position, CharInfo charInfo);
        
        void putString(Short x, Short y, const wchar_t* string);
        void putString(Short x, Short y, const Wchar* string, Attribute attributes);
        void putString(const Coord& position, const CharInfo charInfos[], size_t length);
        void putString(const Coord& position, const CharInfo* charInfos);

        void putMatrix(Short x, Short y, const CharMatrix& matrix);  // todo use Matrix::merge to get the overlap
        void putMatrix(const Coord& position, const CharMatrix& matrix);
    
    private:
        class Impl;
        std::unique_ptr<Impl> impl;
    };
}

#endif  // CINTER_INCLUDE_WINDOW_HPP
