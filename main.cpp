#include "source/include/eventloop.hpp"
#include "source/include/definitions.hpp"
#include "source/include/console.hpp"
#include "source/include/window.hpp"
#include "source/include/application.hpp"

int main()
{
    wci::Window window;
    wci::Console& console = window.console();
    wci::EventLoop eventLoop(console);

    console.cursorInfo(wci::CursorInfo{ 1, false });

    bool proceed = true;
    eventLoop.bindKeyEvent([&](wci::KeyEventRecord keyEvent)
    {
        static wci::Coord position{ 0, 0 };
        static const wci::CharInfo black{ ' ', 0 };
        static const wci::CharInfo character{ '#', static_cast<wci::Word>(wci::Attribute::FgColorCyanBright) };

        window.putChar(position, black);
        if (keyEvent.keyDown)
        {
            if (keyEvent.virtualScanCode == static_cast<wci::Dword>(wci::VirtualKey::Right))
                ++position.x;
            else if (keyEvent.virtualScanCode == static_cast<wci::Dword>(wci::VirtualKey::Left))
                --position.x;
            else if (keyEvent.virtualScanCode == static_cast<wci::Dword>(wci::VirtualKey::Down))
                ++position.y;
            else if (keyEvent.virtualScanCode == static_cast<wci::Dword>(wci::VirtualKey::Up))
                --position.y;
            else if (keyEvent.virtualScanCode == static_cast<wci::Dword>(wci::VirtualKey::Escape))
                proceed = false;
        }
        window.putChar(position, character);
        window.render();
    });
    eventLoop.execute(proceed);

    return 0;
}
