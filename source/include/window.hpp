#pragma once
#ifndef CINTER_INCLUDE_WINDOW_HPP
#define CINTER_INCLUDE_WINDOW_HPP

#include <memory>
#include "definitions.hpp"
#include "console.hpp"

namespace cinter {

    class Window
    {
    public:
        Window();
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        Window(Window&&) = default;
        Window& operator=(Window&&) = default;

        Console& console() noexcept;
        const Console& console() const noexcept;

        CharMatrix& matrix() noexcept;
        const CharMatrix& matrix() const noexcept;

        void render();

        void printChar(wchar_t character);
        void printString(const wchar_t* string);

        void putChar(Short x, Short y, wchar_t character);
        void putChar(Short x, Short y, wchar_t character, Word attributes);
        void putChar(const Coord& position, CharInfo charInfo);
        
        void putString(Short x, Short y, const wchar_t* string);
        void putString(Short x, Short y, const wchar_t* string, Word attributes);
        void putString(const Coord& position, const CharInfo* charInfos, size_t length);

        void putMatrix(Short x, Short y, const CharMatrix& matrix);
        void putMatrix(const Coord& position, const CharMatrix& matrix);
    
    private:
        class Impl;
        std::unique_ptr<Impl> pImpl;
    };
}

#endif  // CINTER_INCLUDE_WINDOW_HPP
