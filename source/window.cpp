#define UNICODE
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
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
    Impl() : inner{ wci::Console(), wci::CharMatrix(0, 0) }
    {
        inner.matrix.resize(inner.console.screenBufferInfo().size);
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

    const wci::Coord& size() const
    {
        return matrix().size();
    }

    void resize()  // idea optimize this
    {
        auto newSize = console().screenBufferInfo().size;
        if (newSize.x == size().x && newSize.y == size().y)  // todo implement operator==
            return;
        wci::CharMatrix newMatrix(newSize);
        newMatrix.inlay(matrix());
        matrix().swap(newMatrix);
    }

    void render()
    {
        resize();
        SMALL_RECT rect{
            0, 0,
            wci::api(size().x) - 1,
            wci::api(size().y) - 1
        };
        WriteConsoleOutputW(
            wci::api(console().stdOutput()),  // check or console().activeScreenBuffer()?
            wci::api(matrix().data()),
            wci::api(matrix().size()),
            COORD{ 0, 0 },
            &rect
        );
    }

    void printChar(Wchar character)  // review
    {
        auto info = console().screenBufferInfo();
        auto position = info.cursorPosition;
        if (character != L'\n')
            matrix().put(position, character, info.attributes);
        if (position.x >= info.size.x || character == L'\n')  // go to the next line
        {
            position.x = 0;
            ++position.y;
        }
        else
            ++position.x;
        console().cursorPosition(position);
    }

    void printString(const Wchar* string)  // review
    {
        auto info = console().screenBufferInfo();
        auto position = info.cursorPosition;
        size_t i = 0;
        while (string[i])
        {
            if (string[i] != L'\n')
                matrix().put(position, string[i], info.attributes);
            if (position.x >= info.size.x || string[i] == L'\n')  // go to next line
            {
                position.x = 0;
                ++position.y;
            }
            else
                ++position.x;
            ++i;
        }
        console().cursorPosition(position);
    }

    void putString(wci::Short x, wci::Short y, const wci::Wchar* string, wci::Attribute attributes)
    {
        wci::Short i = 0;
        while (string[i])
        {
            matrix().put(x + i, y, string[i], attributes);
            ++i;
        }
    }
    void putString(wci::Short x, wci::Short y, const wci::CharInfo charInfos[], size_t length)
    {
        for (size_t i = 0; i < length; ++i)
            matrix().put(x + static_cast<wci::Short>(i), y, charInfos[i]);
    }

    void putMatrix(wci::Short x, wci::Short y, const wci::CharMatrix& matrix)
    {
        inner.matrix.inlay(x, y, matrix);
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

const wci::Coord& wci::Window::size() const
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
    impl->matrix().put(x, y, character, attributes);
}
void wci::Window::putChar(wci::Short x, wci::Short y, wci::Wchar character, wci::Attribute attributes)
{
    impl->matrix().put(x, y, character, attributes);
}
void wci::Window::putChar(const wci::Coord& position, const wci::CharInfo& charInfo)
{
    impl->matrix().put(position, charInfo);
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

void wci::Window::putMatrix(wci::Short x, wci::Short y, const wci::CharMatrix& matrix)
{
    impl->putMatrix(x, y, matrix);
}
void wci::Window::putMatrix(const wci::Coord& position, const wci::CharMatrix& matrix)
{
    impl->putMatrix(position.x, position.y, matrix);
}
