#include "source/include/eventloop.hpp"
#include "source/include/definitions.hpp"
#include "source/include/console.hpp"
#include "source/include/window.hpp"
#include "source/include/application.hpp"

int main()
{
    cinter::Window window;
    cinter::Console& console = window.console();
    cinter::EventLoop eventLoop(console);
    
    console.cursorInfo(cinter::CursorInfo{ 1, false });
    
    bool proceed = true;
    eventLoop.bindKeyEvent([&](cinter::KeyEventRecord keyEvent)
    {
        static cinter::Coord position{ 0, 0 };
        static const cinter::CharInfo black{ ' ', 0 };
        static const cinter::CharInfo character{ '#', static_cast<cinter::Word>(cinter::Attribute::FgColorCyanBright) };
    
        window.putChar(position, black);
        if (keyEvent.keyDown)
        {
            if (keyEvent.virtualScanCode == static_cast<cinter::Dword>(cinter::VirtualKey::Right))
                ++position.x;
            else if (keyEvent.virtualScanCode == static_cast<cinter::Dword>(cinter::VirtualKey::Left))
                --position.x;
            else if (keyEvent.virtualScanCode == static_cast<cinter::Dword>(cinter::VirtualKey::Down))
                ++position.y;
            else if (keyEvent.virtualScanCode == static_cast<cinter::Dword>(cinter::VirtualKey::Up))
                --position.y;
            else if (keyEvent.virtualScanCode == static_cast<cinter::Dword>(cinter::VirtualKey::Escape))
                proceed = false;
        }
        window.putChar(position, character);
        window.render();
    });
    eventLoop.execute(proceed);

    return 0;
}
