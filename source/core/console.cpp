#define UNICODE
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <memory>
#include <string>
#include <vector>
#include <windows.h>

#include "console.hpp"
#include "../util/apicast.hpp"

// todo this needs much polishing...

class wci::Console::Impl
{
private:
    wci::Handle stdIn, stdOut, stdErr;
    struct {  // settings to be restored on destruction
        DWORD mode;
        UINT codePage, outputCodePage;
        wci::CursorInfo cursor;
        wci::Attribute attributes;
    } old;
    std::vector<INPUT_RECORD> rawInputBuffer;  // for input events reading

public:
    Impl()
    {
        if (!wci::Console::attached())
            wci::Console::allocate();

        stdIn = wci::Console::standardHandle(wci::StdHandle::Input);
        stdOut = wci::Console::standardHandle(wci::StdHandle::Output);
        stdErr = wci::Console::standardHandle(wci::StdHandle::Error);

        GetConsoleMode(wci::api(stdIn), &old.mode);
        auto newMode = wci::InputMode(old.mode);
        newMode |= wci::InputMode::EnableExtendedFlags | wci::InputMode::EnableWindowsInput | wci::InputMode::EnableMouseInput;
        newMode &= ~wci::InputMode::EnableQuickEditMode;
        SetConsoleMode(wci::api(stdIn), wci::api(newMode));

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

    void write(char character)
    {
        wchar_t wide = '\0';
        MultiByteToWideChar(CP_UTF8, 0, &character, 1, &wide, 1);
        WriteConsoleW(wci::api(stdOut), &wide, 1, NULL, NULL);
    }
    void write(wchar_t character)
    {
        WriteConsoleW(wci::api(stdOut), &character, 1, NULL, NULL);
    }
    void write(const char* string)
    {
        int wideLength = MultiByteToWideChar(CP_UTF8, 0, string, -1, NULL, 0);
        if (wideLength > 0)
        {
            std::wstring wideString(wideLength, 0);
            MultiByteToWideChar(CP_UTF8, 0, string, -1, wideString.data(), wideLength);  // check
            // if (!wideString.empty() && wideString.back() == L'\0')
                // wideString.pop_back();
            write(wideString.c_str());
        }
    }
    void write(const wchar_t* wstring)
    {
        DWORD length = 0;
        while (wstring[length]) ++length;
        WriteConsoleW(wci::api(stdOut), wstring, length, NULL, NULL);
    }

    std::wstring getTitle() const
    {
        DWORD length = GetConsoleTitleW(NULL, 0);
        if (length == 0)
            return L"";
        std::vector<wchar_t> buffer(length + 1, 0);
        GetConsoleTitleW(wci::api(buffer.data()), wci::api(buffer.size()));
        return std::wstring(buffer.data(), length);
    }
    void setTitle(const std::wstring& newTitle)
    {
        SetConsoleTitleW(wci::api(newTitle.data()));
    }

    wci::ScreenBufferInfo getScreenBufferInfo() const
    {
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(wci::api(stdOut), &info);
        return wci::wci(info);
    }

    wci::CursorInfo getCursorInfo() const
    {
        CONSOLE_CURSOR_INFO info;
        GetConsoleCursorInfo(wci::api(stdOut), &info);
        return wci::wci(info);
    }
    void setCursorInfo(const wci::CursorInfo& info)
    {
        CONSOLE_CURSOR_INFO winInfo = wci::api(info);
        SetConsoleCursorInfo(wci::api(stdOut), &winInfo);
    }

    void setCursorPosition(const wci::Point& position)
    {
        COORD winCoord = wci::api(position);
        SetConsoleCursorPosition(wci::api(stdOut), winCoord);
    }

    wci::Handle getActiveScreenBuffer() const
    {
        return stdOut;
    }
    void setActiveScreenBuffer(wci::Handle handle)
    {
        SetConsoleActiveScreenBuffer(wci::api(handle));
        stdOut = handle;
        stdErr = handle;
    }

    void setTextAttribute(wci::Attribute attributes)
    {
        SetConsoleTextAttribute(wci::api(stdOut), wci::api(attributes));
    }

    void readInput(wci::InputEvent inputBuffer[], wci::Dword inputBufferLength, wci::Dword* eventsRead)
    {
        rawInputBuffer.resize(inputBufferLength);
        auto success = ReadConsoleInputW(
            wci::api(stdIn),
            rawInputBuffer.data(),
            wci::api(inputBufferLength),
            wci::api(eventsRead)
        );
        if (!success)
        {
            *eventsRead = 0;
            return;
        }
        for (wci::Dword i = 0; i < *eventsRead; ++i)
            inputBuffer[i] = wci::wci(rawInputBuffer[i]);
    }

    Handle getStdInput() const
    {
        return stdIn;
    }
    Handle getStdOutput() const
    {
        return stdOut;
    }
    Handle getStdError() const
    {
        return stdErr;
    }
};

void wci::Console::allocate()
{
    AllocConsole();
}

void wci::Console::attach(wci::Dword processId)
{
    AttachConsole(wci::api(processId));
}

void wci::Console::detach()
{
    FreeConsole();
}

bool wci::Console::attached() noexcept
{
    return GetConsoleWindow() != NULL;
}

wci::Handle wci::Console::standardHandle(wci::StdHandle handle)
{
    return wci::wci(GetStdHandle(wci::api(handle)));
}

wci::Handle wci::Console::createScreenBuffer(wci::GenericRights generic, wci::FileAccessRights fileAccess)
{
    return wci::wci(CreateConsoleScreenBuffer(
        wci::api(generic),
        wci::api(fileAccess),
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    ));
}

wci::Console::Console() : impl(std::make_unique<Impl>()) {}

wci::Console::~Console() = default;

void wci::Console::write(char character)
{
    impl->write(character);
}
void wci::Console::write(char character, wci::Attribute attributes)
{
    auto info = impl->getScreenBufferInfo();
    impl->setTextAttribute(attributes);
    impl->write(character);
    impl->setTextAttribute(info.attributes);
}
void wci::Console::write(wci::Wchar character)
{
    impl->write(character);
}
void wci::Console::write(wci::Wchar character, wci::Attribute attributes)
{
    auto info = impl->getScreenBufferInfo();
    impl->setTextAttribute(attributes);
    impl->write(character);
    impl->setTextAttribute(info.attributes);
}
void wci::Console::write(const wci::CharCell& cell)
{
    auto info = impl->getScreenBufferInfo();
    impl->setTextAttribute(cell.attributes);
    impl->write(cell.character);
    impl->setTextAttribute(info.attributes);
}
void wci::Console::write(const char* string)
{
    impl->write(string);
}
void wci::Console::write(const char* string, wci::Attribute attributes)
{
    auto info = impl->getScreenBufferInfo();
    impl->setTextAttribute(attributes);
    impl->write(string);
    impl->setTextAttribute(info.attributes);
}
void wci::Console::write(const std::string& string)
{
    impl->write(string.c_str());
}
void wci::Console::write(const std::string& string, wci::Attribute attributes)
{
    auto info = impl->getScreenBufferInfo();
    impl->setTextAttribute(attributes);
    impl->write(string.c_str());
    impl->setTextAttribute(info.attributes);
}
void wci::Console::write(const wchar_t* wstring)
{
    impl->write(wstring);
}
void wci::Console::write(const wchar_t* wstring, wci::Attribute attributes)
{
    auto info = impl->getScreenBufferInfo();
    impl->setTextAttribute(attributes);
    impl->write(wstring);
    impl->setTextAttribute(info.attributes);
}
void wci::Console::write(const std::wstring& wstring)
{
    impl->write(wstring.c_str());
}
void wci::Console::write(const std::wstring& wstring, wci::Attribute attributes)
{
    auto info = impl->getScreenBufferInfo();
    impl->setTextAttribute(attributes);
    impl->write(wstring.c_str());
    impl->setTextAttribute(info.attributes);
}
void wci::Console::write(const wci::CharString& string)
{
    auto info = impl->getScreenBufferInfo();
    for (const auto& cell : string)  // todo optimize
    {
        impl->setTextAttribute(cell.attributes);
        impl->write(cell.character);
    }
    impl->setTextAttribute(info.attributes);
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

void wci::Console::cursorPosition(const wci::Point& position)
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

void wci::Console::readInput(wci::InputEvent inputBuffer[], wci::Dword inputBufferLength, wci::Dword* eventsRead)
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

wci::Console& wci::Console::operator<<(const wci::Point& position)
{
    cursorPosition(position);
    return *this;
}
wci::Console& wci::Console::operator<<(wci::Attribute attributes)
{
    textAttribute(attributes);
    return *this;
}
wci::Console& wci::Console::operator<<(wci::Reset)
{
    textAttribute(wci::normal);
    return *this;
}
wci::Console& wci::Console::operator<<(char character)
{
    write(character);
    return *this;
}
wci::Console& wci::Console::operator<<(wci::Wchar character)
{
    write(character);
    return *this;
}
wci::Console& wci::Console::operator<<(const wci::CharCell& cell)
{
    write(cell);
    return *this;
}
wci::Console& wci::Console::operator<<(const std::string& string)
{
    write(string);
    return *this;
}
wci::Console& wci::Console::operator<<(const std::wstring& string)
{
    write(string);
    return *this;
}
wci::Console& wci::Console::operator<<(const CharString& string)
{
    write(string);
    return *this;
}
