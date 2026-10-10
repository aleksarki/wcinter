#pragma once

#include "../wci.hpp"

#include "options.hpp"

using namespace wci::literals;

class MainPage : public wci::Page
{
    wci::Navigator& navigator;
    wci::FrameLoop& loop;
    wci::MenuList menu;

public:
    MainPage(wci::Window& window, wci::Navigator& navigator, wci::FrameLoop& loop) :
        Page(window), navigator(navigator), loop(loop), menu()
    {
        menu.addOption(L"Start New Game");
        menu.addOption(L"Game Options");
        menu.addOption(L"Game Stats");
        menu.addOption(L"About the Game");
        menu.addOption(L"Exit");
        menu.optionJustify(wci::Justify::Center);
        menu.selectionCharLeft(wci::cell( L'>'));
        menu.selectionCharRight(wci::cell(L'<'));
        menu.index(0);
    }

    void build() override
    {
        place(menu, 50_rel, 50_rel, wci::Anchor::Center);
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

        case wci::VirtualKey::Return:
        case wci::VirtualKey::Space:
            switch (menu.index())
            {
            case 1:
                navigator.requestEmplace<OptionsPage>(window, navigator);
                return true;

            case 4:
                loop.stop();
                return true;
            }
        }

        return false;
    }
};
