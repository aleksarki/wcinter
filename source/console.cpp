#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>
#include <string>
#include <vector>
#include "include/definitions.hpp"
#include "include/winapi.hpp"
#include "include/console.hpp"

class wci::Console::Impl
{
private:
    HANDLE hStdIn, hStdOut, hStdErr;
    DWORD oldConsoleMode;
    UINT oldConsoleCP, oldConsoleOutputCP;

public:
    Impl()
    {
        hStdIn = GetStdHandle(STD_INPUT_HANDLE);
        hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        hStdErr = GetStdHandle(STD_ERROR_HANDLE);
        GetConsoleMode(hStdIn, &oldConsoleMode);
        constexpr auto newConsoleMode = wci::api(wci::InputMode::EnableWindowsInput | wci::InputMode::EnableMouseInput);
        SetConsoleMode(hStdIn, oldConsoleMode | newConsoleMode);
        oldConsoleCP = GetConsoleCP();
        oldConsoleOutputCP = GetConsoleOutputCP();
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
    }

    ~Impl()
    {
        SetConsoleMode(hStdIn, oldConsoleMode);
        SetConsoleCP(oldConsoleCP);
        SetConsoleOutputCP(oldConsoleOutputCP);
    }

    void write(const char* string)
    {
        int wideLength = MultiByteToWideChar(CP_UTF8, 0, string, -1, NULL, 0);
        if (wideLength > 0)
        {
            std::wstring wideString(wideLength, 0);
            MultiByteToWideChar(CP_UTF8, 0, string, -1, &wideString[0], wideLength);
            // if (!wideString.empty() && wideString.back() == L'\0')
                // wideString.pop_back();
            write(wideString.c_str());
        }
    }
    void write(const wchar_t* wstring)
    {
        DWORD length = 0;
        while (wstring[length]) ++length;
        WriteConsoleW(hStdOut, wstring, length, NULL, NULL);
    }

    std::wstring getTitle()
    {
        DWORD length = GetConsoleTitleW(NULL, 0);
        if (length == 0)
            return L"";
        
        std::vector<wchar_t> buffer(length + 1, 0);
        GetConsoleTitleW(buffer.data(), static_cast<DWORD>(buffer.size()));

        return std::wstring(buffer.data());
    }
    void setTitle(const std::wstring& newTitle)
    {
        SetConsoleTitleW(newTitle.data());
    }

    wci::ScreenBufferInfo getScreenBufferInfo()
    {
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(hStdOut, &info);
        return wci::ScreenBufferInfo{
            wci::Coord{
                static_cast<wci::Short>(info.dwSize.X),
                static_cast<wci::Short>(info.dwSize.Y)
            },
            wci::Coord{
                static_cast<wci::Short>(info.dwCursorPosition.X),
                static_cast<wci::Short>(info.dwCursorPosition.Y)
            },
            static_cast<wci::Attribute>(info.wAttributes),
            wci::SmallRect{
                static_cast<wci::Short>(info.srWindow.Left),
                static_cast<wci::Short>(info.srWindow.Top),
                static_cast<wci::Short>(info.srWindow.Right),
                static_cast<wci::Short>(info.srWindow.Bottom)
            },
            wci::Coord{
                static_cast<wci::Short>(info.dwMaximumWindowSize.X),
                static_cast<wci::Short>(info.dwMaximumWindowSize.Y)
            }
        };
    }
    
    wci::CursorInfo getCursorInfo()
    {
        CONSOLE_CURSOR_INFO info;
        GetConsoleCursorInfo(hStdOut, &info);
        return wci::CursorInfo{
            static_cast<wci::Dword>(info.dwSize),
            static_cast<bool>(info.bVisible)
        };
    }
    void setCursorInfo(const wci::CursorInfo& info)
    {
        CONSOLE_CURSOR_INFO winInfo{
            static_cast<DWORD>(info.size),
            static_cast<BOOL>(info.visible)
        };
        SetConsoleCursorInfo(hStdOut, &winInfo);
    }

    void setCursorPosition(const wci::Coord& position)
    {
        COORD winCoord{
            static_cast<SHORT>(position.x),
            static_cast<SHORT>(position.y)
        };
        SetConsoleCursorPosition(hStdOut, winCoord);
    }

    wci::Handle getActiveScreenBuffer()
    {
        return hStdOut;
    }
    void setActiveScreenBuffer(wci::Handle handle)
    {
        SetConsoleActiveScreenBuffer(static_cast<HANDLE>(handle));
        hStdOut = handle;
        hStdErr = handle;
    }

    void setTextAttribute(wci::Word attributes)
    {
        SetConsoleTextAttribute(hStdOut, static_cast<WORD>(attributes));
    }

    void writeMatrix(const wci::CharMatrix& matrix)
    {
        auto info = getScreenBufferInfo();
        SMALL_RECT rect{
            0, 0,
            static_cast<SHORT>(info.size.x),
            static_cast<SHORT>(info.size.y)
        };
        WriteConsoleOutputW(
            hStdOut,
            reinterpret_cast<CONST CHAR_INFO*>(matrix.data()),
            COORD{
                static_cast<SHORT>(matrix.size().x),
                static_cast<SHORT>(matrix.size().y)
            },
            COORD{ 0, 0 },
            &rect
        );
    }

    void readInput(wci::InputRecord* inputBuffer, wci::Dword inputBufferLength, wci::Dword* eventsRead)
    {
        ReadConsoleInputW(
            hStdIn,
            reinterpret_cast<PINPUT_RECORD>(inputBuffer),  /* this is definitely not good */
            static_cast<DWORD>(inputBufferLength),
            reinterpret_cast<LPDWORD>(eventsRead)
        );
    }
};

wci::Console::Console() : pImpl(std::make_unique<Impl>()) {}
wci::Console::~Console() = default;

void wci::Console::write(const char* string)
{
    pImpl->write(string);
}
void wci::Console::write(const std::string& string)
{
    pImpl->write(string.c_str());
}
void wci::Console::write(const wchar_t* wstring)
{
    pImpl->write(wstring);
}
void wci::Console::write(const std::wstring& wstring)
{
    pImpl->write(wstring.c_str());
}

std::wstring wci::Console::title()
{
    return pImpl->getTitle();
}
void wci::Console::title(const std::wstring& newTitle)
{
    pImpl->setTitle(newTitle);
}

wci::ScreenBufferInfo wci::Console::screenBufferInfo()
{
    return pImpl->getScreenBufferInfo();
}

wci::CursorInfo wci::Console::cursorInfo()
{
    return pImpl->getCursorInfo();
}
void wci::Console::cursorInfo(const wci::CursorInfo& info)
{
    pImpl->setCursorInfo(info);
}

void wci::Console::cursorPosition(const wci::Coord& position)
{
    pImpl->setCursorPosition(position);
}

wci::Handle wci::Console::activeScreenBuffer()
{
    return pImpl->getActiveScreenBuffer();
}
void wci::Console::activeScreenBuffer(wci::Handle handle)
{
    pImpl->setActiveScreenBuffer(handle);
}

void wci::Console::textAttribute(Word attributes)
{
    pImpl->setTextAttribute(attributes);
}

void wci::Console::writeMatrix(const CharMatrix& matrix)
{
    pImpl->writeMatrix(matrix);
}

void wci::Console::readInput(wci::InputRecord* inputBuffer, wci::Dword inputBufferLength, wci::Dword* eventsRead)
{
    pImpl->readInput(inputBuffer, inputBufferLength, eventsRead);
}
