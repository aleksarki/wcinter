#include "source/include/eventloop.hpp"
#include "source/include/definitions.hpp"
#include "source/include/console.hpp"
#include "source/include/window.hpp"

int main()
{
    cinter::Window window;
    cinter::EventLoop EventLoop(window.console());

    EventLoop.bindKeyEvent([&window](cinter::KeyEventRecord keyEvent)
    {
        window.putChar(0, 0, keyEvent.character, static_cast<cinter::Word>(cinter::Attribute::FgColorCyan));
        window.render();
    });

    EventLoop.execute();

    return 0;
}
