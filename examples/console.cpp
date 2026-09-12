#include <iostream>
#include <random>
#include <string>
#include "../source/include/console.hpp"
#include "../source/include/definitions.hpp"

int main()
{
    wci::Console console;
    auto print = [&console](const std::string& title, const std::wstring& message)
    {
        console.write(title + ": ");
        console.write(message + L'\n');
    };

    console.write(L"test\n");
    console.write(L"тест\n");
    console.write(L"tête\n");
    console.write(L"τεςτ\n");

    print("Current title is", console.title());
    console.title(L"New Title");

    std::getchar();

    console.title(L"New New Title");
    print("Current title is", console.title());

    std::getchar();

    {
        wci::ScreenBufferInfo info = console.screenBufferInfo();
        print("Console width is", std::to_wstring(info.size.x));
        print("Console height is", std::to_wstring(info.size.y));
        print("Cursor is at", std::to_wstring(info.cursorPosition.x) + L"x" + std::to_wstring(info.cursorPosition.y));
        print("Window top left corner is", std::to_wstring(info.window.left) + L"x" + std::to_wstring(info.window.top));
        print("Window bottom right corner is", std::to_wstring(info.window.right) + L"x" + std::to_wstring(info.window.bottom));
        print("Maximum window width is", std::to_wstring(info.maxWindowSize.x));
        print("Maximum window height is", std::to_wstring(info.maxWindowSize.y));
    }

    std::getchar();

    {
        wci::CursorInfo info = console.cursorInfo();

        print("Cursor size is", std::to_wstring(info.size));
        std::getchar();

        console.cursorInfo({ 5, true });
        info = console.cursorInfo();
        print("Cursor size is", std::to_wstring(info.size));
        std::getchar();

        console.cursorInfo({ 50, true });
        info = console.cursorInfo();
        print("Cursor size is", std::to_wstring(info.size));
        std::getchar();

        print("Cursor is", info.visible ? L"visible" : L"invisible");
        std::getchar();

        console.cursorInfo({ 25, false });
        info = console.cursorInfo();
        print("Cursor is", info.visible ? L"visible" : L"invisible");
        std::getchar();

        console.cursorInfo({ 25, true });
        info = console.cursorInfo();
        print("Cursor is", info.visible ? L"visible" : L"invisible");
    }

    std::getchar();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<short> dist(1, 20);

    short x = dist(gen), y = dist(gen);
    console.textAttribute(wci::Attribute::BgColorBlack | wci::Attribute::FgColorWhite | wci::Attribute::CommonLvbReverseVideo);
    console.cursorPosition({ x, y });
    console.write("black|white");
    std::getchar();

    x = dist(gen), y = dist(gen);
    console.textAttribute(wci::Attribute::BgColorBlueBright | wci::Attribute::FgColorMagentaBright);
    console.cursorPosition({ x, y });
    console.write("blue bright|magenta bright");
    std::getchar();

    x = dist(gen), y = dist(gen);
    console.textAttribute(wci::Attribute::FgColorCyan | wci::Attribute::BgColorYellowBright);
    console.cursorPosition({ x, y });
    console.write("yellow bright|cyan");
    std::getchar();

    x = dist(gen), y = dist(gen);
    console.textAttribute(wci::Attribute::BgColorBlackBright | wci::Attribute::FgColorGreenBright);
    console.cursorPosition({ x, y });
    console.write("black bright|green bright");
    std::getchar();

    x = dist(gen), y = dist(gen);
    console.textAttribute(wci::Attribute::BgColorMagenta | wci::Attribute::FgColorRedBright);
    console.cursorPosition({ x, y });
    console.write("magenta|red bright");
    std::getchar();

    console.textAttribute(
        wci::Attribute::BgColorBlue | wci::Attribute::FgColorYellow |
        wci::Attribute::CommonLvbGridHorizontal | wci::Attribute::CommonLvbUnderscore
    );
    print("stdin is", std::to_wstring((int)console.stdInput()));
    print("stdout is", std::to_wstring((int)console.stdOutput()));
    print("stderr is", std::to_wstring((int)console.stdError()));

    std::getchar();
    return 0;
}
