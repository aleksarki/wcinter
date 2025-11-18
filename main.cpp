#include "source/include/definitions.hpp"
#include "source/include/console.hpp"
#include "source/include/window.hpp"

#include <iostream>

int main()
{
    cinter::Window window;
    window.console().textAttribute(static_cast<cinter::Word>(cinter::Attribute::FgColorCyan));
    window.printString(L"Hello there!\n");
    window.printString(L"Hello there!\n");
    window.render();

    int a;
    std::cin >> a;

    return 0;
}
