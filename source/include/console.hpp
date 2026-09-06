#pragma once
#ifndef CINTER_INCLUDE_CONSOLE_HPP
#define CINTER_INCLUDE_CONSOLE_HPP

#include <memory>
#include <string>
#include "definitions.hpp"

namespace wci {

    class Console
    {
    public:
        Console();
        ~Console();

        Console(const Console&) = delete;
        Console& operator=(const Console&) = delete;

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

        // Get console title.
        std::wstring title();

        // Set new console title.
        void title(const std::wstring& newTitle);

        // Get wci::ScreenBufferInfo object describing console's screen buffer.
        ScreenBufferInfo screenBufferInfo();

        CursorInfo cursorInfo();
        void cursorInfo(const CursorInfo& info);

        void cursorPosition(const Coord& position);

        Handle activeScreenBuffer();
        void activeScreenBuffer(Handle handle);

        void textAttribute(Word attributes);

        void writeMatrix(const CharMatrix& matrix);

        void readInput(InputRecord* inputBuffer, Dword inputBufferLength, Dword* eventsRead);

    private:
        class Impl;
        std::unique_ptr<Impl> impl;
    };
}

#endif  // CINTER_INCLUDE_CONSOLE_HPP
