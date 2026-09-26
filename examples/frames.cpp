#include <chrono>
#include <string>
#include <utility>
#include "../source/core.hpp"
#include "../source/defs.hpp"

using namespace std::chrono_literals;

int main()
{
    wci::Window window;
    wci::FrameLoop loop(window.console());

    window.console().cursorInfo({ 1, false });  // todo make this automatic

    wci::Coord point{ 0, 0 };
    wci::CharMatrix plate(100, 8);
    int counter = 0;

    auto msg1 = wci::CharString(L"Counter: ");
    auto msg2 = wci::CharString(L"Tap ↑ to increase frames per second by at least 1.").toMatrix();
    auto msg3 = wci::CharString(L"Tap ↓ to increase frame time by 10ms.").toMatrix();
    auto msg4 = wci::CharString(L"Use WASD to move this message.").toMatrix();
    auto msg5 = wci::CharString(L"Current frames per second: ");
    auto msg6 = wci::CharString(L"Current frame time: ");

    loop.bindOnKeyEvent([&](const wci::KeyEventRecord& event)
    {
        if (!event.keyDown)
            return;
        switch (event.virtualKeyCode)
        {
        case wci::VirtualKey::Escape:
            loop.stop();
            return;
        case wci::VirtualKey('A'):
            --point.x;
            break;
        case wci::VirtualKey('D'):
            ++point.x;
            break;
        case wci::VirtualKey('W'):
            --point.y;
            break;
        case wci::VirtualKey('S'):
            ++point.y;
            break;
        case wci::VirtualKey::Up:
            loop.framesPerSecond(loop.framesPerSecond() + 1);
            break;
        case wci::VirtualKey::Down:
            loop.frameTime(loop.frameTime() + 10ms);
            break;
        }
    });

    loop.bindOnTick([&]()
    {
        window.matrix().blank();  // todo make Window:: method

        plate.blank();
        plate.inlay(0, 0, (msg1 + std::to_wstring(counter++)).toMatrix());
        plate.inlay(0, 2, msg2);
        plate.inlay(0, 3, msg3);
        plate.inlay(0, 4, msg4);
        plate.inlay(0, 6, (msg5 + std::to_wstring(loop.framesPerSecond())).toMatrix());
        plate.inlay(0, 7, (msg6 + std::to_wstring(loop.frameTime().count())).toMatrix());

        window.put(point, plate);
        window.render();
    });

    loop.run();

    return 0;
}
