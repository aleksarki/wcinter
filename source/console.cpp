#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>
#include <string>
#include <vector>
#include "include/structs.hpp"
#include "include/console.hpp"

namespace ci = cinter;

class ci::Console::Impl
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
        SetConsoleMode(hStdIn, oldConsoleMode | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
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

    ci::ScreenBufferInfo getScreenBufferInfo()
    {
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(hStdOut, &info);
        return ci::ScreenBufferInfo{
            ci::Coord{
                static_cast<ci::Short>(info.dwSize.X),
                static_cast<ci::Short>( info.dwSize.Y)
            },
            ci::Coord{
                static_cast<ci::Short>(info.dwCursorPosition.X),
                static_cast<ci::Short>(info.dwCursorPosition.Y)
            },
            static_cast<ci::Word>(info.wAttributes),
            ci::SmallRect{
                static_cast<ci::Short>(info.srWindow.Left),
                static_cast<ci::Short>(info.srWindow.Top),
                static_cast<ci::Short>(info.srWindow.Right),
                static_cast<ci::Short>(info.srWindow.Bottom)
            },
            ci::Coord{
                static_cast<ci::Short>(info.dwMaximumWindowSize.X),
                static_cast<ci::Short>(info.dwMaximumWindowSize.Y)
            }
        };
    }
    
    ci::CursorInfo getCursorInfo()
    {
        CONSOLE_CURSOR_INFO info;
        GetConsoleCursorInfo(hStdOut, &info);
        return ci::CursorInfo{
            static_cast<ci::Dword>(info.dwSize),
            static_cast<bool>(info.bVisible)
        };
    }
    void setCursorInfo(const ci::CursorInfo& info)
    {
        CONSOLE_CURSOR_INFO winInfo{
            static_cast<DWORD>(info.size),
            static_cast<int>(info.visible)
        };
        SetConsoleCursorInfo(hStdOut, &winInfo);
    }

    void setCursorPosition(const ci::Coord& position)
    {
        COORD winCoord{
            static_cast<SHORT>(position.x),
            static_cast<SHORT>(position.y)
        };
        SetConsoleCursorPosition(hStdOut, winCoord);
    }
};

ci::Console::Console() : pImpl(std::make_unique<Impl>()) {}
ci::Console::~Console() = default;

void ci::Console::write(const char* string)
{
    pImpl->write(string);
}
void ci::Console::write(const std::string& string)
{
    pImpl->write(string.c_str());
}
void ci::Console::write(const wchar_t* wstring)
{
    pImpl->write(wstring);
}
void ci::Console::write(const std::wstring& wstring)
{
    pImpl->write(wstring.c_str());
}

std::wstring ci::Console::title()
{
    return pImpl->getTitle();
}
void ci::Console::title(const std::wstring& newTitle)
{
    pImpl->setTitle(newTitle);
}

ci::ScreenBufferInfo ci::Console::screenBufferInfo()
{
    return pImpl->getScreenBufferInfo();
}

ci::CursorInfo ci::Console::cursorInfo()
{
    return pImpl->getCursorInfo();
}
void ci::Console::cursorInfo(const ci::CursorInfo& info)
{
    pImpl->setCursorInfo(info);
}

void ci::Console::cursorPosition(const ci::Coord& position)
{
    pImpl->setCursorPosition(position);
}
