#include <memory>
#include <windows.h>
#include "include/console.hpp"

namespace ci = cinter;

class ci::Console::Impl
{
private:
    HANDLE hStdIn, hStdOut, hStdErr;
    DWORD oldConsoleMode;

public:
    Impl()
    {
        hStdIn = GetStdHandle(STD_INPUT_HANDLE);
        hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        hStdErr = GetStdHandle(STD_ERROR_HANDLE);
        GetConsoleMode(hStdIn, &oldConsoleMode);
        SetConsoleMode(hStdIn, oldConsoleMode | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
    }

    ~Impl()
    {
        SetConsoleMode(hStdIn, oldConsoleMode);
    }
};

ci::Console::Console() : pImpl(std::make_unique<Impl>()) {}
ci::Console::~Console() = default;
