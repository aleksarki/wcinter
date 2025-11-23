#include "source/include/application.hpp"
#include "source/include/definitions.hpp"
#include "source/include/console.hpp"
#include "source/include/window.hpp"

int main()
{
    cinter::Window window;
    cinter::Application application(window.console());

    window.console().textAttribute(static_cast<cinter::Word>(cinter::Attribute::FgColorCyanBright));

    application.bindKeyEvent([&window](cinter::KeyEventRecord keyEvent)
    {
        if (keyEvent.keyDown)
            window.printChar(keyEvent.virtualScanCode);
        window.render();
    });

    application.execute();

    return 0;
}
