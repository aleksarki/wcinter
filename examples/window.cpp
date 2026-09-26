#include "../source/core.hpp"
#include "../source/defs.hpp"

int main()
{
    wci::Window window;

    window.console().cursorInfo({ 1, false });

    window.matrix()[{ 20, 10 }] = wci::CharInfo{ L'A', wci::Attribute::FgColorGreen };
    window.put(21, 10, L"bcdefg", wci::Attribute::FgColorBlue);

    window.render();
    std::getchar();

    return 0;
}
