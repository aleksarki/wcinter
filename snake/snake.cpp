#include <memory>
#include "../wci.hpp"

#include "main.hpp"

using namespace wci::literals;

int main()
{
    wci::Window window;
    wci::FrameLoop loop(window.console());
    wci::Navigator navigator;

    navigator.emplace<MainPage>(window, navigator, loop);

    loop.bindOnTick([&]()
    {
        if (navigator.dispatchTick())
            window.render();
    });
    loop.bindOnKeyEvent([&](const wci::KeyEvent& event)
    {
        navigator.dispatchKeyEvent(event);
    });
    loop.run();

    return 0;
}
