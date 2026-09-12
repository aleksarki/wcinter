#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>
#include <string>
#include <vector>
#include "include/apicast.hpp"
#include "include/console.hpp"
#include "include/definitions.hpp"

class wci::Console::Impl
{
private:
    HANDLE stdIn, stdOut, stdErr;
    struct {  // settings to be restored on destruction
        DWORD mode;
        UINT codePage, outputCodePage;
        wci::CursorInfo cursor;
        wci::Attribute attributes;
    } old;

public:
    Impl()
    {
        stdIn = GetStdHandle(STD_INPUT_HANDLE);
        stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        stdErr = GetStdHandle(STD_ERROR_HANDLE);
        GetConsoleMode(stdIn, &old.mode);
        SetConsoleMode(stdIn, old.mode | wci::api(wci::InputMode::EnableWindowsInput | wci::InputMode::EnableMouseInput));
        old.codePage = GetConsoleCP();
        old.outputCodePage = GetConsoleOutputCP();
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
        old.cursor = getCursorInfo();
        old.attributes = getScreenBufferInfo().attributes;
    }
    ~Impl()
    {
        SetConsoleMode(stdIn, old.mode);
        SetConsoleCP(old.codePage);
        SetConsoleOutputCP(old.outputCodePage);
        setCursorInfo(old.cursor);
        setTextAttribute(old.attributes);
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
        WriteConsoleW(stdOut, wstring, length, NULL, NULL);
    }

    std::wstring getTitle() const
    {
        DWORD length = GetConsoleTitleW(NULL, 0);
        if (length == 0)
            return L"";
        std::vector<wchar_t> buffer(length + 1, 0);
        GetConsoleTitleW(wci::api(buffer.data()), wci::api(buffer.size()));
        return std::wstring(buffer.data());
    }
    void setTitle(const std::wstring& newTitle)
    {
        SetConsoleTitleW(wci::api(newTitle.data()));
    }

    wci::ScreenBufferInfo getScreenBufferInfo() const
    {
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(stdOut, &info);
        return wci::wci(info);
    }

    wci::CursorInfo getCursorInfo() const
    {
        CONSOLE_CURSOR_INFO info;
        GetConsoleCursorInfo(stdOut, &info);
        return wci::wci(info);
    }
    void setCursorInfo(const wci::CursorInfo& info)
    {
        CONSOLE_CURSOR_INFO winInfo = wci::api(info);
        SetConsoleCursorInfo(stdOut, &winInfo);
    }

    void setCursorPosition(const wci::Coord& position)
    {
        COORD winCoord = wci::api(position);
        SetConsoleCursorPosition(stdOut, winCoord);
    }

    wci::Handle getActiveScreenBuffer() const
    {
        return wci::wci(stdOut);
    }
    void setActiveScreenBuffer(wci::Handle handle)
    {
        SetConsoleActiveScreenBuffer(wci::api(handle));
        stdOut = handle;
        stdErr = handle;
    }

    void setTextAttribute(wci::Attribute attributes)
    {
        SetConsoleTextAttribute(stdOut, wci::api(attributes));
    }

    void readInput(wci::InputRecord* inputBuffer, wci::Dword inputBufferLength, wci::Dword* eventsRead)
    {
        ReadConsoleInputW(
            stdIn,
            reinterpret_cast<PINPUT_RECORD>(inputBuffer),  /* fixme: this is definitely not good */
            wci::api(inputBufferLength),
            wci::api(eventsRead)
        );
    }

    Handle getStdInput() const
    {
        return wci::wci(stdIn);
    }
    Handle getStdOutput() const
    {
        return wci::wci(stdOut);
    }
    Handle getStdError() const
    {
        return wci::wci(stdErr);
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

std::wstring wci::Console::title() const
{
    return impl->getTitle();
}
void wci::Console::title(const std::wstring& newTitle)
{
    impl->setTitle(newTitle);
}

wci::ScreenBufferInfo wci::Console::screenBufferInfo() const
{
    return impl->getScreenBufferInfo();
}

wci::CursorInfo wci::Console::cursorInfo() const
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

wci::Handle wci::Console::activeScreenBuffer() const
{
    return impl->getActiveScreenBuffer();
}
void wci::Console::activeScreenBuffer(wci::Handle handle)
{
    impl->setActiveScreenBuffer(handle);
}

void wci::Console::textAttribute(wci::Attribute attributes)
{
    impl->setTextAttribute(attributes);
}

void wci::Console::readInput(wci::InputRecord* inputBuffer, wci::Dword inputBufferLength, wci::Dword* eventsRead)
{
    impl->readInput(inputBuffer, inputBufferLength, eventsRead);
}

wci::Handle wci::Console::stdInput() const
{
    return impl->getStdInput();
}
wci::Handle wci::Console::stdOutput() const
{
    return impl->getStdOutput();
}
wci::Handle wci::Console::stdError() const
{
    return impl->getStdError();
}
