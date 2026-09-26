#include "../source/core.hpp"
#include "../source/defs.hpp"

int main()
{
    wci::Window window;

    wci::CharMatrix matrix1(40, 10);
    matrix1.fill(L' ', wci::Attribute::BgColorBlue);

    wci::CharMatrix matrix2(12, 6);
    matrix2.fill(L' ', wci::Attribute::BgColorCyanBright);

    wci::CharMatrix matrix3(4, 2);
    auto attr = wci::Attribute::BgColorGreenBright | wci::Attribute::FgColorYellowBright;
    matrix3.fill(L'═', attr);
    matrix3[{ 0, 0 }] = { L'╔', attr };
    matrix3[{ 3, 0 }] = { L'╗', attr };
    matrix3[{ 0, 1 }] = { L'╚', attr };
    matrix3[{ 3, 1 }] = { L'╝', attr };

    matrix2.inlay(2, 1, matrix3);
    matrix2.inlay(5, 2, matrix3);
    matrix2.inlay(3, 3, matrix3);
    matrix1.inlay(4, 2, matrix2);

    window.put(0, 1, L"dummy srirgn", wci::Attribute::FgColorYellow);
    window.put(1, 1, matrix1);

    window.render();

    std::getchar();
    return 0;
}
