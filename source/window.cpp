#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
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
