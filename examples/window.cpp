#include "../source/include/definitions.hpp"
#include "../source/include/window.hpp"

int main()
{
    wci::Window window;

    window.console().cursorInfo({ 1, false });

    window.matrix()[{ 20, 10 }] = wci::CharInfo{ L'A', wci::Attribute::FgColorGreen };
    window.putString(21, 10, L"bcdefg", wci::Attribute::FgColorBlue);

    window.render();
    std::getchar();

    return 0;
}
