#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>
#include <string>
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
            write(wideString.c_str());
        }
    }

    void write(const wchar_t* wstring)
    {
        DWORD length = 0;
        while (wstring[length]) ++length;
        WriteConsoleW(hStdOut, wstring, length, NULL, NULL);
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
