#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>
#include "include/definitions.hpp"
#include "include/console.hpp"
#include "include/window.hpp"

class wci::Window::Impl
{
private:
    wci::Console con;
    wci::Coord size;
    wci::Handle oldScreenBuffer;
    wci::CharMatrix mat;

public:
    Impl() : con(), mat(con.screenBufferInfo().size)
    {
        oldScreenBuffer = con.activeScreenBuffer();
        HANDLE handle = CreateConsoleScreenBuffer(
            static_cast<DWORD>(GenericRights::Read) | static_cast<DWORD>(GenericRights::Write),
            static_cast<DWORD>(FileAccessRights::ShareRead) | static_cast<DWORD>(FileAccessRights::ShareWrite),
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL
        );
        con.activeScreenBuffer(static_cast<wci::Handle>(handle));
    }

    ~Impl()
    {
        con.activeScreenBuffer(oldScreenBuffer);
    }

    wci::Console& console() noexcept
    {
        return con;
    }
    const wci::Console& console() const noexcept
    {
        return con;
    }

    wci::CharMatrix& matrix() noexcept
    {
        return mat;
    }
    const wci::CharMatrix& matrix() const noexcept
    {
        return mat;
    }

    void render()
    {
        con.writeMatrix(mat);
    }

    void printChar(wchar_t character)
    {
        auto info = con.screenBufferInfo();
        auto position = info.cursorPosition;
        if (character != L'\n')
            mat.put(position, character, info.attributes);
        if (position.x >= info.size.x || character == L'\n')  // go to next line
        {
            position.x = 0;
            ++position.y;
        }
        else
            ++position.x;
        con.cursorPosition(position);
    }
    void printString(const wchar_t* string)
    {
        auto info = con.screenBufferInfo();
        auto position = info.cursorPosition;
        unsigned length = 0;
        while (string[length])
        {
            if (string[length] != L'\n')
                mat.put(position, string[length], info.attributes);
            if (position.x >= info.size.x || string[length] == L'\n')  // go to next line
            {
                position.x = 0;
                ++position.y;
            }
            else
                ++position.x;
            ++length;
        }
        con.cursorPosition(position);
    }

    void putChar(wci::Short x, wci::Short y, wchar_t character)
    {
        mat.put(x, y, character, con.screenBufferInfo().attributes);
    }
    void putChar(wci::Short x, wci::Short y, wchar_t character, wci::Word attributes)
    {
        mat.put(x, y, character, attributes);
    }

    void putString(wci::Short x, wci::Short y, const wchar_t* string)
    {
        auto attributes = con.screenBufferInfo().attributes;
        unsigned length = 0;
        while (string[length])
        {
            mat.put(x + length, y, string[length], attributes);
            ++length;
        }
    }
    void putString(wci::Short x, wci::Short y, const wchar_t* string, wci::Word attributes)
    {
        unsigned length = 0;
        while (string[length])
        {
            mat.put(x + length, y, string[length], attributes);
            ++length;
        }
    }
    void putString(wci::Short x, wci::Short y, const wci::CharInfo* charInfos, size_t length)
    {
        for (size_t i = 0; i < length; ++i)
            mat.put(x + static_cast<wci::Short>(i), y, charInfos[i]);
    }
};

wci::Window::Window() : pImpl(std::make_unique<Impl>()) {}
wci::Window::~Window() = default;

wci::Console& wci::Window::console() noexcept
{
    return pImpl->console();
}
const wci::Console& wci::Window::console() const noexcept
{
    return pImpl->console();
}

wci::CharMatrix& wci::Window::matrix() noexcept
{
    return pImpl->matrix();
}
const wci::CharMatrix& wci::Window::matrix() const noexcept
{
    return pImpl->matrix();
}

void wci::Window::render()
{
    pImpl->render();
}

void wci::Window::printChar(wchar_t character)
{
    pImpl->printChar(character);
}
void wci::Window::printString(const wchar_t* string)
{
    pImpl->printString(string);
}

void wci::Window::putChar(wci::Short x, wci::Short y, wchar_t character)
{
    pImpl->putChar(x, y, character);
}
void wci::Window::putChar(wci::Short x, wci::Short y, wchar_t character, wci::Word attributes)
{
    pImpl->putChar(x, y, character, attributes);
}
void wci::Window::putChar(const wci::Coord& position, wci::CharInfo charInfo)
{
    pImpl->putChar(position.x, position.y, charInfo.character, charInfo.attributes);
}

void wci::Window::putString(wci::Short x, wci::Short y, const wchar_t* string)
{
    pImpl->putString(x, y, string);
}
void wci::Window::putString(wci::Short x, wci::Short y, const wchar_t* string, wci::Word attributes)
{
    pImpl->putString(x, y, string, attributes);
}
void wci::Window::putString(const Coord& position, const CharInfo* charInfos, size_t length)
{
    pImpl->putString(position.x, position.y, charInfos, length);
}
