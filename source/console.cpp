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
            ci::Coord{ info.dwSize.X, info.dwSize.Y },
            ci::Coord{ info.dwCursorPosition.X, info.dwCursorPosition.Y },
            info.wAttributes,
            ci::SmallRect{ info.srWindow.Left, info.srWindow.Top, info.srWindow.Right, info.srWindow.Bottom },
            ci::Coord{ info.dwMaximumWindowSize.X, info.dwMaximumWindowSize.Y }
        };
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
