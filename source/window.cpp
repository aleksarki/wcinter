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

    const wci::Coord size() const
    {
        return inner.matrix.size();
    }

    void resize()
    {
        // fixme update inner matrix size, use Matrix::merge; on buffer resize recreate underlying matrix
        inner.matrix = wci::CharMatrix(inner.console.screenBufferInfo().size);
    }

    void render()
    {
        // resize();  // fixme
        SMALL_RECT rect{
            0, 0,
            wci::api(size().x),
            wci::api(size().y)
        };
        WriteConsoleOutputW(
            wci::api(inner.console.stdOutput()),
            wci::api(inner.matrix.data()),
            wci::api(inner.matrix.size()),
            COORD{ 0, 0 },
            &rect
        );
    }

    void printChar(Wchar character)  // review
    {
        auto info = inner.console.screenBufferInfo();
        auto position = info.cursorPosition;
        if (character != L'\n')
            inner.matrix.put(position, character, info.attributes);
        if (position.x >= info.size.x || character == L'\n')  // go to the next line
        {
            position.x = 0;
            ++position.y;
        }
        else
            ++position.x;
        inner.console.cursorPosition(position);
    }

    void printString(const Wchar* string)
    {
        auto info = inner.console.screenBufferInfo();
        auto position = info.cursorPosition;
        size_t i = 0;
        while (string[i])
        {
            if (string[i] != L'\n')
                inner.matrix.put(position, string[i], info.attributes);
            if (position.x >= info.size.x || string[i] == L'\n')  // go to next line
            {
                position.x = 0;
                ++position.y;
            }
            else
                ++position.x;
            ++i;
        }
        inner.console.cursorPosition(position);
    }

    void putChar(wci::Short x, wci::Short y, wci::Wchar character, wci::Attribute attributes)
    {
        inner.matrix.put(x, y, character, attributes);
    }

    void putString(wci::Short x, wci::Short y, const wci::Wchar* string, wci::Attribute attributes)
    {
        unsigned i = 0;
        while (string[i])
        {
            inner.matrix.put(x + i, y, string[i], attributes);
            ++i;
        }
    }
    void putString(wci::Short x, wci::Short y, const wci::CharInfo charInfos[], size_t length)
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

const wci::Coord wci::Window::size() const
{
    return impl->size();
}

void wci::Window::resize()
{
    impl->resize();
}

void wci::Window::render()
{
    impl->render();
}

void wci::Window::printChar(wci::Wchar character)
{
    impl->printChar(character);
}

void wci::Window::printString(const wci::Wchar* string)
{
    impl->printString(string);
}
void wci::Window::printString(const std::wstring& string)
{
    impl->printString(string.data());
}

void wci::Window::putChar(wci::Short x, wci::Short y, wci::Wchar character)
{
    auto attributes = impl->console().screenBufferInfo().attributes;
    impl->putChar(x, y, character, attributes);
}
void wci::Window::putChar(wci::Short x, wci::Short y, wci::Wchar character, wci::Attribute attributes)
{
    impl->putChar(x, y, character, attributes);
}
void wci::Window::putChar(const wci::Coord& position, wci::CharInfo charInfo)
{
    impl->putChar(position.x, position.y, charInfo.character, charInfo.attributes);
}

void wci::Window::putString(wci::Short x, wci::Short y, const wci::Wchar* string)
{
    auto attributes = impl->console().screenBufferInfo().attributes;
    impl->putString(x, y, string, attributes);
}
void wci::Window::putString(wci::Short x, wci::Short y, const wci::Wchar* string, wci::Attribute attributes)
{
    impl->putString(x, y, string, attributes);
}
void wci::Window::putString(const wci::Coord& position, const wci::CharInfo charInfos[], size_t length)
{
    impl->putString(position.x, position.y, charInfos, length);
}
