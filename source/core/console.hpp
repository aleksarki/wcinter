#pragma once
#ifndef WCI_SOURCE_CORE_CONSOLE_HPP
#define WCI_SOURCE_CORE_CONSOLE_HPP

#include <memory>
#include <string>

#include "../defs.hpp"

namespace wci {

    class Console
    {
    public:
        Console();
        ~Console();

        Console(const Console&) = delete;
        Console& operator=(const Console&) = delete;

        // Write to console.

        void write(char character);
        void write(char character, Attribute attributes);
        void write(Wchar character);
        void write(Wchar character, Attribute attributes);
        void write(const CharInfo& charInfo);
        void write(const char* string);
        void write(const char* string, Attribute attributes);
        void write(const std::string& string);
        void write(const std::string& string, Attribute attributes);
        void write(const wchar_t* wstring);
        void write(const wchar_t* wstring, Attribute attributes);
        void write(const std::wstring& wstring);
        void write(const std::wstring& wstring, Attribute attributes);
        void write(const CharString& string);

        // Read from console.  -- (put off) --

        /*
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
        */

        // Get console title.
        std::wstring title() const;

        // Set new console title.
        void title(const std::wstring& newTitle);

        // Get wci::ScreenBufferInfo object describing console's screen buffer.
        ScreenBufferInfo screenBufferInfo() const;

        CursorInfo cursorInfo() const;
        void cursorInfo(const CursorInfo& info);

        void cursorPosition(const Coord& position);

        Handle activeScreenBuffer() const;
        void activeScreenBuffer(Handle handle);

        void textAttribute(Attribute attributes);

        void readInput(InputRecord inputBuffer[], Dword inputBufferLength, Dword* eventsRead);

        Handle stdInput() const;

        Handle stdOutput() const;

        Handle stdError() const;

        Console& operator<<(const Coord& position);
        Console& operator<<(Attribute attributes);
        Console& operator<<(Reset);
        Console& operator<<(char character);
        Console& operator<<(Wchar character);
        Console& operator<<(const CharInfo& charInfo);
        Console& operator<<(const std::string& string);
        Console& operator<<(const std::wstring& string);
        Console& operator<<(const CharString& string);

    private:
        class Impl;
        std::unique_ptr<Impl> impl;
    };
}

#endif  // WCI_SOURCE_CORE_CONSOLE_HPP
