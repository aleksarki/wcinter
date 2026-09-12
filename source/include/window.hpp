#pragma once
#ifndef CINTER_INCLUDE_WINDOW_HPP
#define CINTER_INCLUDE_WINDOW_HPP

#include <memory>
#include <string>
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

        const Coord size() const;

        void resize();
        
        void render();

        void printChar(Wchar character);

        void printString(const Wchar* string);
        void printString(const std::wstring& string);

        void putChar(Short x, Short y, Wchar character);
        void putChar(Short x, Short y, Wchar character, Attribute attributes);
        void putChar(const Coord& position, CharInfo charInfo);
        
        void putString(Short x, Short y, const Wchar* string);
        void putString(Short x, Short y, const Wchar* string, Attribute attributes);
        void putString(const Coord& position, const CharInfo charInfos[], size_t length);

        void putMatrix(Short x, Short y, const CharMatrix& matrix);  // todo use Matrix::merge to get the overlap
        void putMatrix(const Coord& position, const CharMatrix& matrix);
    
    private:
        class Impl;
        std::unique_ptr<Impl> impl;
    };
}

#endif  // CINTER_INCLUDE_WINDOW_HPP
