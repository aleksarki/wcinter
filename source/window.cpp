#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>
#include "include/apicast.hpp"
#include "include/console.hpp"
#include "include/definitions.hpp"
#include "include/window.hpp"

class wci::Window::Impl
{
private:
    struct {
        wci::Console console;
        wci::CharMatrix matrix;
    } inner;
    struct {  // setting to be restored on destruction
        wci::Handle screenBuffer;
    } old;

public:
    Impl() : inner{ wci::Console(), wci::CharMatrix(inner.console.screenBufferInfo().size) }
    {
        old.screenBuffer = inner.console.activeScreenBuffer();
        HANDLE handle = CreateConsoleScreenBuffer(
            wci::api(GenericRights::Read | GenericRights::Write),
            wci::api(FileAccessRights::ShareRead | FileAccessRights::ShareWrite),
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL
        );
        inner.console.activeScreenBuffer(wci::wci(handle));
    }
    ~Impl()
    {
        inner.console.activeScreenBuffer(old.screenBuffer);
    }

    wci::Console& console() noexcept
    {
        return inner.console;
    }
    const wci::Console& console() const noexcept
    {
        return inner.console;
    }

    wci::CharMatrix& matrix() noexcept
    {
        return inner.matrix;
    }
    const wci::CharMatrix& matrix() const noexcept
    {
        return inner.matrix;
    }

    void render()
    {
        auto info = inner.console.screenBufferInfo();
        SMALL_RECT rect{
            0, 0,
            wci::api(info.size.x),
            wci::api(info.size.y)
        };
        WriteConsoleOutputW(
            wci::api(inner.console.stdOutput()),
            wci::api(inner.matrix.data()),
            wci::api(inner.matrix.size()),
            COORD{ 0, 0 },
            &rect
        );
    }

    void printChar(wchar_t character)
    {
        auto info = inner.console.screenBufferInfo();
        auto position = info.cursorPosition;
        if (character != L'\n')
            inner.matrix.put(position, character, info.attributes);
        if (position.x >= info.size.x || character == L'\n')  // go to next line
        {
            position.x = 0;
            ++position.y;
        }
        else
            ++position.x;
        inner.console.cursorPosition(position);
    }
    void printString(const wchar_t* string)
    {
        auto info = inner.console.screenBufferInfo();
        auto position = info.cursorPosition;
        unsigned length = 0;
        while (string[length])
        {
            if (string[length] != L'\n')
                inner.matrix.put(position, string[length], info.attributes);
            if (position.x >= info.size.x || string[length] == L'\n')  // go to next line
            {
                position.x = 0;
                ++position.y;
            }
            else
                ++position.x;
            ++length;
        }
        inner.console.cursorPosition(position);
    }

    void putChar(wci::Short x, wci::Short y, wchar_t character)
    {
        inner.matrix.put(x, y, character, inner.console.screenBufferInfo().attributes);
    }
    void putChar(wci::Short x, wci::Short y, wchar_t character, wci::Attribute attributes)
    {
        inner.matrix.put(x, y, character, attributes);
    }

    void putString(wci::Short x, wci::Short y, const wchar_t* string)
    {
        auto attributes = inner.console.screenBufferInfo().attributes;
        unsigned length = 0;
        while (string[length])
        {
            inner.matrix.put(x + length, y, string[length], attributes);
            ++length;
        }
    }
    void putString(wci::Short x, wci::Short y, const wchar_t* string, wci::Attribute attributes)
    {
        unsigned length = 0;
        while (string[length])
        {
            inner.matrix.put(x + length, y, string[length], attributes);
            ++length;
        }
    }
    void putString(wci::Short x, wci::Short y, const wci::CharInfo* charInfos, size_t length)
    {
        for (size_t i = 0; i < length; ++i)
            inner.matrix.put(x + static_cast<wci::Short>(i), y, charInfos[i]);
    }
};

wci::Window::Window() : impl(std::make_unique<Impl>()) {}
wci::Window::~Window() = default;

wci::Console& wci::Window::console() noexcept
{
    return impl->console();
}
const wci::Console& wci::Window::console() const noexcept
{
    return impl->console();
}

wci::CharMatrix& wci::Window::matrix() noexcept
{
    return impl->matrix();
}
const wci::CharMatrix& wci::Window::matrix() const noexcept
{
    return impl->matrix();
}

void wci::Window::render()
{
    impl->render();
}

void wci::Window::printChar(wchar_t character)
{
    impl->printChar(character);
}
void wci::Window::printString(const wchar_t* string)
{
    impl->printString(string);
}

void wci::Window::putChar(wci::Short x, wci::Short y, wchar_t character)
{
    impl->putChar(x, y, character);
}
void wci::Window::putChar(wci::Short x, wci::Short y, wchar_t character, wci::Attribute attributes)
{
    impl->putChar(x, y, character, attributes);
}
void wci::Window::putChar(const wci::Coord& position, wci::CharInfo charInfo)
{
    impl->putChar(position.x, position.y, charInfo.character, charInfo.attributes);
}

void wci::Window::putString(wci::Short x, wci::Short y, const wchar_t* string)
{
    impl->putString(x, y, string);
}
void wci::Window::putString(wci::Short x, wci::Short y, const wchar_t* string, wci::Attribute attributes)
{
    impl->putString(x, y, string, attributes);
}
void wci::Window::putString(const Coord& position, const CharInfo* charInfos, size_t length)
{
    impl->putString(position.x, position.y, charInfos, length);
}
