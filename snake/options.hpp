#pragma once

#include "../wci.hpp"

using namespace wci::literals;

class OptionsPage : public wci::Page
{
    wci::Navigator& navigator;
    wci::MenuList menu;

public:
    OptionsPage(wci::Window& window, wci::Navigator& navigator) : wci::Page(window), navigator(navigator), menu()
    {
        menu.addOption(L"Pick snake color");
        menu.addOption(L"Pick food color");
        menu.addOption(L"Set snake speed");
        menu.addOption(L"Set speed acceleration");
        menu.addOption(L"Set food spawning rate");
        menu.addOption(L"Set food aging");
        menu.addOption(L"Reset highscores");
        menu.addOption(L"Back");
        menu.optionJustify(wci::Justify::Center);
        menu.selectionCharLeft(wci::cell( L'>'));
        menu.selectionCharRight(wci::cell(L'<'));
        menu.index(0);
    }

    void build() override
    {
        place(50_rel, 50_rel, menu, wci::Anchor::Center);
    }

    bool onKeyEvent(const wci::KeyEvent& event) override
    {
        if (!event.keyDown)
            return false;
        
        switch (event.virtualKeyCode)
        {
        case wci::VirtualKey::Up:
        case wci::VirtualKey(L'W'):
            menu.prev();
            return true;

        case wci::VirtualKey::Down:
        case wci::VirtualKey(L'S'):
            menu.next();
            return true;

        case wci::VirtualKey::Escape:
            navigator.requestPop();
            return true;

        case wci::VirtualKey::Return:
        case wci::VirtualKey::Space:
            switch (menu.index())
            {
            case 7:
                navigator.requestPop();
                return true;
            }
        }

        return false;
    }
};
