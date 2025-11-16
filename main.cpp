#include "source/include/definitions.hpp"
#include "source/include/console.hpp"
#include "source/include/window.hpp"

#include <iostream>

int main()
{
    cinter::Window window;
    window.console().cursorInfo({ 1, false });
    window.matrix().put(0, 0, L'H', static_cast<cinter::Word>(cinter::Attribute::FgColorBlack));
    window.matrix().put(1, 0, L'i', static_cast<cinter::Word>(cinter::Attribute::FgColorBlue));
    window.matrix().put(3, 0, L't', static_cast<cinter::Word>(cinter::Attribute::FgColorGreen));
    window.matrix().put(4, 0, L'h', static_cast<cinter::Word>(cinter::Attribute::FgColorCyan));
    window.matrix().put(5, 0, L'e', static_cast<cinter::Word>(cinter::Attribute::FgColorRed));
    window.matrix().put(6, 0, L'r', static_cast<cinter::Word>(cinter::Attribute::FgColorMagenta));
    window.matrix().put(7, 0, L'e', static_cast<cinter::Word>(cinter::Attribute::FgColorYellow));
    window.matrix().put(8, 0, L'!', static_cast<cinter::Word>(cinter::Attribute::FgColorWhite));

    window.matrix().put(0, 1, L'H', static_cast<cinter::Word>(cinter::Attribute::FgColorBlackBright));
    window.matrix().put(1, 1, L'i', static_cast<cinter::Word>(cinter::Attribute::FgColorBlueBright));
    window.matrix().put(3, 1, L't', static_cast<cinter::Word>(cinter::Attribute::FgColorGreenBright));
    window.matrix().put(4, 1, L'h', static_cast<cinter::Word>(cinter::Attribute::FgColorCyanBright));
    window.matrix().put(5, 1, L'e', static_cast<cinter::Word>(cinter::Attribute::FgColorRedBright));
    window.matrix().put(6, 1, L'r', static_cast<cinter::Word>(cinter::Attribute::FgColorMagentaBright));
    window.matrix().put(7, 1, L'e', static_cast<cinter::Word>(cinter::Attribute::FgColorYellowBright));
    window.matrix().put(8, 1, L'!', static_cast<cinter::Word>(cinter::Attribute::FgColorWhiteBright));
    window.render();

    int a;
    std::cin >> a;

    return 0;
}
