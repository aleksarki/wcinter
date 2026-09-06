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
    struct {  // setting to be restored on destruction
        DWORD mode;
        UINT codePage, outputCodePage;
    } old;

public:
    Impl()
    {
        hStdIn = GetStdHandle(STD_INPUT_HANDLE);
        hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        hStdErr = GetStdHandle(STD_ERROR_HANDLE);
        GetConsoleMode(hStdIn, &old.mode);
        SetConsoleMode(hStdIn, old.mode | wci::api(wci::InputMode::EnableWindowsInput | wci::InputMode::EnableMouseInput));
        old.codePage = GetConsoleCP();
        old.outputCodePage = GetConsoleOutputCP();
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
    }
    ~Impl()
    {
        SetConsoleMode(hStdIn, old.mode);
        SetConsoleCP(old.codePage);
        SetConsoleOutputCP(old.outputCodePage);
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
        return wci::wci(info);
    }

    wci::CursorInfo getCursorInfo()
    {
        CONSOLE_CURSOR_INFO info;
        GetConsoleCursorInfo(hStdOut, &info);
        return wci::wci(info);
    }
    void setCursorInfo(const wci::CursorInfo& info)
    {
        CONSOLE_CURSOR_INFO winInfo = wci::api(info);
        SetConsoleCursorInfo(hStdOut, &winInfo);
    }

    void setCursorPosition(const wci::Coord& position)
    {
        COORD winCoord = wci::api(position);
        SetConsoleCursorPosition(hStdOut, winCoord);
    }

    wci::Handle getActiveScreenBuffer()
    {
        return hStdOut;
    }
    void setActiveScreenBuffer(wci::Handle handle)
    {
        SetConsoleActiveScreenBuffer(wci::api(handle));
        hStdOut = handle;
        hStdErr = handle;
    }

    void setTextAttribute(wci::Word attributes)
    {
        SetConsoleTextAttribute(hStdOut, wci::api(attributes));
    }

    void writeMatrix(const wci::CharMatrix& matrix)
    {
        auto info = getScreenBufferInfo();
        SMALL_RECT rect{
            0, 0,
            wci::api(info.size.x),
            wci::api(info.size.y)
        };
        WriteConsoleOutputW(
            hStdOut,
            wci::api(matrix.data()),
            wci::api(matrix.size()),
            COORD{ 0, 0 },
            &rect
        );
    }

    void readInput(wci::InputRecord* inputBuffer, wci::Dword inputBufferLength, wci::Dword* eventsRead)
    {
        ReadConsoleInputW(
            hStdIn,
            reinterpret_cast<PINPUT_RECORD>(inputBuffer),  /* fixme: this is definitely not good */
            wci::api(inputBufferLength),
            wci::api(eventsRead)
        );
    }
};

wci::Console::Console() : impl(std::make_unique<Impl>()) {}
wci::Console::~Console() = default;

void wci::Console::write(const char* string)
{
    impl->write(string);
}
void wci::Console::write(const std::string& string)
{
    impl->write(string.c_str());
}
void wci::Console::write(const wchar_t* wstring)
{
    impl->write(wstring);
}
void wci::Console::write(const std::wstring& wstring)
{
    impl->write(wstring.c_str());
}

std::wstring wci::Console::title()
{
    return impl->getTitle();
}
void wci::Console::title(const std::wstring& newTitle)
{
    impl->setTitle(newTitle);
}

wci::ScreenBufferInfo wci::Console::screenBufferInfo()
{
    return impl->getScreenBufferInfo();
}

wci::CursorInfo wci::Console::cursorInfo()
{
    return impl->getCursorInfo();
}
void wci::Console::cursorInfo(const wci::CursorInfo& info)
{
    impl->setCursorInfo(info);
}

void wci::Console::cursorPosition(const wci::Coord& position)
{
    impl->setCursorPosition(position);
}

wci::Handle wci::Console::activeScreenBuffer()
{
    return impl->getActiveScreenBuffer();
}
void wci::Console::activeScreenBuffer(wci::Handle handle)
{
    impl->setActiveScreenBuffer(handle);
}

void wci::Console::textAttribute(Word attributes)
{
    impl->setTextAttribute(attributes);
}

void wci::Console::writeMatrix(const CharMatrix& matrix)
{
    impl->writeMatrix(matrix);
}

void wci::Console::readInput(wci::InputRecord* inputBuffer, wci::Dword inputBufferLength, wci::Dword* eventsRead)
{
    impl->readInput(inputBuffer, inputBufferLength, eventsRead);
}
