#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>
#include "include/definitions.hpp"
#include "include/window.hpp"

namespace ci = cinter;

class ci::Window::Impl
{
private:
    ci::Console console;
    ci::Coord size;
    std::vector<ci::CharInfo> matrix;
    HANDLE oldScreenBuffer;

    ci::CharInfo& charAt(ci::Short x, ci::Short y) noexcept
    {
        return matrix[y * size.x + x];
    }
    const ci::CharInfo& charAt(ci::Short x, ci::Short y) const noexcept
    {
        return matrix[y * size.x + x];
    }
    void charAt(ci::Short x, ci::Short y, ci::CharInfo charInfo)
    {
        matrix[y * size.x + x] = charInfo;
    }

public:
    Impl() : console(), matrix()
    {
        auto screenBufferInfo = console.screenBufferInfo();
        size = screenBufferInfo.size;
        matrix.resize(size.x * size.y);
        oldScreenBuffer = console.activeScreenBuffer();
        HANDLE handle = CreateConsoleScreenBuffer(
            static_cast<DWORD>(GenericRights::Read) | static_cast<DWORD>(GenericRights::Write),
            static_cast<DWORD>(FileAccessRights::ShareRead) | static_cast<DWORD>(FileAccessRights::ShareWrite),
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL
        );
        console.activeScreenBuffer(static_cast<ci::Handle>(handle));
    }

    ~Impl()
    {
        console.activeScreenBuffer(oldScreenBuffer);
    }
};

ci::Window::Window() : pImpl(std::make_unique<Impl>()) {}
ci::Window::~Window() = default;
