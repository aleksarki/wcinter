#include "../source/include/console.hpp"
#include "../source/include/definitions.hpp"
#include "../source/include/eventloop.hpp"
#include "../source/include/window.hpp"

int main()
{
    wci::Window window;
    wci::Console& console = window.console();
    wci::EventLoop eventLoop(console);

    console.cursorInfo({ 1, false });

    bool proceed = true;
    eventLoop.bindKeyEvent([&](const wci::KeyEventRecord& keyEvent)
    {
        static wci::Coord position{ 0, 0 };
        static const wci::CharInfo black{ ' ', wci::Attribute::No };
        static const wci::CharInfo character{ '#', wci::Attribute::FgColorCyanBright };

        window.putChar(position, black);
        if (!keyEvent.keyDown)
            return;

        switch (keyEvent.virtualKeyCode)
        {
        case wci::VirtualKey::Right:
            ++position.x;
            break;
        case wci::VirtualKey::Left:
            --position.x;
            break;
        case wci::VirtualKey::Down:
            ++position.y;
            break;
        case wci::VirtualKey::Up:
            --position.y;
            break;
        case wci::VirtualKey::Escape:
            proceed = false;
            return;
        }

        window.putChar(position, character);
        window.render();
    });
    eventLoop.execute(proceed);

    return 0;
}
