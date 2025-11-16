#pragma once
#ifndef CINTER_INCLUDE_CONSOLE_HPP
#define CINTER_INCLUDE_CONSOLE_HPP

#include <memory>
#include <string>
#include "structs.hpp"

namespace cinter {

    class Console
    {
    private:
        class Impl;
        std::unique_ptr<Impl> pImpl;

    public:
        Console();
        ~Console();

        Console(const Console&) = delete;
        Console& operator=(const Console&) = delete;

        Console(Console&&) = default;
        Console& operator=(Console&&) = default;

        // Write to console.

        void write(const char* string);
        void write(const std::string& string);
        void write(const wchar_t* wstring);
        void write(const std::wstring& wstring);

        // Read from console.  -- (put off) --

        void read(wchar_t* buffer, size_t bufferLength, wchar_t stopChar = '\n');
        void read(wchar_t* buffer, size_t bufferLength, size_t charsToRead);
        template <size_t N>
        void read(wchar_t (&buffer)[N], wchar_t stopChar = '\n') {
            read(buffer, N, stopChar);
        }
        template <size_t N>
        void read(wchar_t (&buffer)[N], size_t charsToRead) {
            read(buffer, N, charsToRead);
        }

        // Get & set console title.

        std::wstring title();
        void title(const std::wstring& newTitle);

        // other

        ScreenBufferInfo screenBufferInfo();

        CursorInfo cursorInfo();
        void cursorInfo(const CursorInfo& info);

        void cursorPosition(const Coord& position);

    };
}

#endif  // CINTER_INCLUDE_CONSOLE_HPP
