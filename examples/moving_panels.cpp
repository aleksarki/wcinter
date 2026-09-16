#include "../source/include/definitions.hpp"
#include "../source/include/eventloop.hpp"
#include "../source/include/window.hpp"

int main()
{
    wci::Window window;
    window.console().cursorInfo({ 1, false });

    wci::CharMatrix panelBottom(20, 10), panelTop(10, 5);
    panelBottom.fill(L' ', wci::Attribute::BgColorBlue);
    panelTop.fill(L' ', wci::Attribute::BgColorYellow);

    wci::Coord positionTop{ 0, 0 }, positionBottom{ 0, 0 };

    auto draw = [&]()
    {
        window.matrix().blank();
        window.putMatrix(positionBottom, panelBottom.overlay(positionTop, panelTop));
        window.render();
    };
    draw();

    wci::EventLoop loop(window.console());

    bool proceed = true;
    loop.bindKeyEvent([&](const wci::KeyEventRecord& event)
    {
        if (!event.keyDown)
            return;

        switch (event.virtualScanCode)
        {
        case wci::VirtualKey::Escape:
            proceed = false;
            return;
        case wci::VirtualKey::Left:
            --positionTop.x;
            break;
        case wci::VirtualKey::Right:
            ++positionTop.x;
            break;
        case wci::VirtualKey::Up:
            --positionTop.y;
            break;
        case wci::VirtualKey::Down:
            ++positionTop.y;
            break;
        case wci::VirtualKey('A'):
            --positionBottom.x;
            break;
        case wci::VirtualKey('D'):
            ++positionBottom.x;
            break;
        case wci::VirtualKey('W'):
            --positionBottom.y;
            break;
        case wci::VirtualKey('S'):
            ++positionBottom.y;
            break;
        }

        draw();
    });
    loop.execute(proceed);

    return 0;
}
