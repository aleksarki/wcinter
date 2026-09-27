#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include "../source/core.hpp"
#include "../source/defs.hpp"

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

    console
        << wci::at(dist(gen), dist(gen))
        << (wci::Attribute::BgColorBlack | wci::Attribute::FgColorWhite | wci::Attribute::CommonLvbReverseVideo)
        << "black|white";
    std::getchar();

    console
        << wci::at(dist(gen), dist(gen))
        << (wci::Attribute::BgColorBlueBright | wci::Attribute::FgColorMagentaBright)
        << "blue bright|magenta bright";
    std::getchar();

    console
        << wci::at(dist(gen), dist(gen))
        << (wci::Attribute::FgColorCyan | wci::Attribute::BgColorYellowBright)
        << "yellow bright|cyan";
    std::getchar();

    console
        << wci::at(dist(gen), dist(gen))
        << (wci::Attribute::BgColorBlackBright | wci::Attribute::FgColorGreenBright)
        << "black bright|green bright";
    std::getchar();

    console
        << wci::at(dist(gen), dist(gen))
        << (wci::Attribute::BgColorMagenta | wci::Attribute::FgColorRedBright)
        << "magenta|red bright";
    std::getchar();

    console.textAttribute(
        wci::Attribute::BgColorBlue | wci::Attribute::FgColorYellow |
        wci::Attribute::CommonLvbGridHorizontal | wci::Attribute::CommonLvbUnderscore
    );
    print("stdin is", std::to_wstring(reinterpret_cast<std::uintptr_t>(console.stdInput())));
    print("stdout is", std::to_wstring(reinterpret_cast<std::uintptr_t>(console.stdOutput())));
    print("stderr is", std::to_wstring(reinterpret_cast<std::uintptr_t>(console.stdError())));

    std::getchar();
    return 0;
}
