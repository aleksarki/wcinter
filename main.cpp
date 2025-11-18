#include "source/include/definitions.hpp"
#include "source/include/console.hpp"
#include "source/include/window.hpp"

#include <iostream>

int main()
{
    cinter::Window window;
    window.console().textAttribute(static_cast<cinter::Word>(cinter::Attribute::FgColorCyan));
    window.putString(10, 10, L"Hi there!");
    window.putString(10, 11, L"Hi there!", static_cast<cinter::Word>(cinter::Attribute::FgColorCyanBright));
    window.render();

    int a;
    std::cin >> a;

    return 0;
}
