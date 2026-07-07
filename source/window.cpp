#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>
#include "include/definitions.hpp"
#include "include/console.hpp"
#include "include/window.hpp"

namespace ci = cinter;

class ci::Window::Impl
{
private:
    ci::Console con;
    ci::Coord size;
    ci::Handle oldScreenBuffer;
    ci::CharMatrix mat;

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
        con.activeScreenBuffer(static_cast<ci::Handle>(handle));
    }

    ~Impl()
    {
        con.activeScreenBuffer(oldScreenBuffer);
    }

    ci::Console& console() noexcept
    {
        return con;
    }
    const ci::Console& console() const noexcept
    {
        return con;
    }

    ci::CharMatrix& matrix() noexcept
    {
        return mat;
    }
    const ci::CharMatrix& matrix() const noexcept
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

    void putChar(ci::Short x, ci::Short y, wchar_t character)
    {
        mat.put(x, y, character, con.screenBufferInfo().attributes);
    }
    void putChar(ci::Short x, ci::Short y, wchar_t character, ci::Word attributes)
    {
        mat.put(x, y, character, attributes);
    }

    void putString(ci::Short x, ci::Short y, const wchar_t* string)
    {
        auto attributes = con.screenBufferInfo().attributes;
        unsigned length = 0;
        while (string[length])
        {
            mat.put(x + length, y, string[length], attributes);
            ++length;
        }
    }
    void putString(ci::Short x, ci::Short y, const wchar_t* string, ci::Word attributes)
    {
        unsigned length = 0;
        while (string[length])
        {
            mat.put(x + length, y, string[length], attributes);
            ++length;
        }
    }
    void putString(ci::Short x, ci::Short y, const ci::CharInfo* charInfos, size_t length)
    {
        for (size_t i = 0; i < length; ++i)
            mat.put(x + static_cast<ci::Short>(i), y, charInfos[i]);
    }
};

ci::Window::Window() : pImpl(std::make_unique<Impl>()) {}
ci::Window::~Window() = default;

ci::Console& ci::Window::console() noexcept
{
    return pImpl->console();
}
const ci::Console& ci::Window::console() const noexcept
{
    return pImpl->console();
}

ci::CharMatrix& ci::Window::matrix() noexcept
{
    return pImpl->matrix();
}
const ci::CharMatrix& ci::Window::matrix() const noexcept
{
    return pImpl->matrix();
}

void ci::Window::render()
{
    pImpl->render();
}

void ci::Window::printChar(wchar_t character)
{
    pImpl->printChar(character);
}
void ci::Window::printString(const wchar_t* string)
{
    pImpl->printString(string);
}

void ci::Window::putChar(ci::Short x, ci::Short y, wchar_t character)
{
    pImpl->putChar(x, y, character);
}
void ci::Window::putChar(ci::Short x, ci::Short y, wchar_t character, ci::Word attributes)
{
    pImpl->putChar(x, y, character, attributes);
}
void ci::Window::putChar(const ci::Coord& position, ci::CharInfo charInfo)
{
    pImpl->putChar(position.x, position.y, charInfo.character, charInfo.attributes);
}

void ci::Window::putString(ci::Short x, ci::Short y, const wchar_t* string)
{
    pImpl->putString(x, y, string);
}
void ci::Window::putString(ci::Short x, ci::Short y, const wchar_t* string, ci::Word attributes)
{
    pImpl->putString(x, y, string, attributes);
}
void ci::Window::putString(const Coord& position, const CharInfo* charInfos, size_t length)
{
    pImpl->putString(position.x, position.y, charInfos, length);
}
