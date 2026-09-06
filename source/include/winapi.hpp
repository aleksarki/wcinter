#pragma once
#ifndef CINTER_INCLUDE_WINAPI_HPP
#define CINTER_INCLUDE_WINAPI_HPP

#include <windows.h>
#include "./definitions.hpp"

namespace wci
{
    #pragma region api
    #pragma region types

    constexpr WCHAR api(Wchar wch)
    {
        return static_cast<WCHAR>(wch);
    }

    constexpr SHORT api(Short sh)
    {
        return static_cast<SHORT>(sh);
    }

    constexpr WORD api(Word w)
    {
        return static_cast<WORD>(w);
    }

    constexpr DWORD api(Dword dw)
    {
        return static_cast<DWORD>(dw);
    }

    constexpr HANDLE api(Handle h)
    {
        return static_cast<HANDLE>(h);
    }

    constexpr BOOL api(bool b)
    {
        return static_cast<BOOL>(b);
    }

    constexpr UINT api(unsigned int ui)
    {
        return static_cast<UINT>(ui);
    }

    #pragma endregion

    #pragma region enums

    constexpr DWORD api(StdHandle sh)
    {
        return static_cast<DWORD>(sh);
    }

    constexpr int api(InputMode im)
    {
        return static_cast<int>(im);
    }

    constexpr int api(CodePage cp)
    {
        return static_cast<int>(cp);
    }

    constexpr WORD api(Attribute a)
    {
        return static_cast<WORD>(a);
    }

    constexpr DWORD api(GenericRights gr)
    {
        return static_cast<DWORD>(gr);
    }

    constexpr DWORD api(FileAccessRights far_)
    {
        return static_cast<DWORD>(far_);
    }

    constexpr DWORD api(ControlKeyState cks)
    {
        return static_cast<DWORD>(cks);
    }

    constexpr DWORD api(ButtonState bs)
    {
        return static_cast<DWORD>(bs);
    }

    constexpr DWORD api(EventFlag ef)
    {
        return static_cast<DWORD>(ef);
    }

    constexpr WORD api(EventType et)
    {
        return static_cast<WORD>(et);
    }

    constexpr WORD api(VirtualKey vk)
    {
        return static_cast<WORD>(vk);
    }

    #pragma endregion

    #pragma region structs

    constexpr COORD api(const Coord& c)
    {
        return COORD{ api(c.x), api(c.y) };
    }

    constexpr SMALL_RECT api(const SmallRect& sr)
    {
        return SMALL_RECT{ api(sr.left), api(sr.top), api(sr.right), api(sr.bottom) };
    }

    constexpr CHAR_INFO api(const CharInfo& chi)
    {
        CHAR_INFO info;
        info.Char.UnicodeChar = api(chi.character);
        info.Attributes = api(chi.attributes);
        return info;
    }

    constexpr CONSOLE_CURSOR_INFO api(const CursorInfo& ci)
    {
        return CONSOLE_CURSOR_INFO{ api(ci.size), api(ci.visible) };
    }

    constexpr CONSOLE_SCREEN_BUFFER_INFO api(const ScreenBufferInfo& sbi)
    {
        return CONSOLE_SCREEN_BUFFER_INFO{
            api(sbi.size),
            api(sbi.cursorPosition),
            api(sbi.attributes),
            api(sbi.window),
            api(sbi.maxWindowSize)
        };
    }

    constexpr KEY_EVENT_RECORD api(const KeyEventRecord ker)
    {
        KEY_EVENT_RECORD record;
        record.bKeyDown = api(ker.keyDown);
        record.wRepeatCount = api(ker.repeatCount);
        record.wVirtualKeyCode = api(ker.virtualKeyCode);
        record.wVirtualScanCode = api(ker.virtualScanCode);
        record.uChar.UnicodeChar = api(ker.character);
        record.dwControlKeyState = api(ker.controlKeyState);
        return record;
    }

    constexpr MOUSE_EVENT_RECORD api(const MouseEventRecord& mer)
    {
        return MOUSE_EVENT_RECORD{
            api(mer.mousePosition),
            api(mer.buttonState),
            api(mer.controlKeyState),
            api(mer.eventFlags)
        };
    }

    constexpr WINDOW_BUFFER_SIZE_RECORD api(const WindowBufferSizeRecord& wbsr)
    {
        return WINDOW_BUFFER_SIZE_RECORD{ api(wbsr.size) };
    }

    constexpr MENU_EVENT_RECORD api(const MenuEventRecord& mer)
    {
        return MENU_EVENT_RECORD{ api(mer.commandId) };
    }

    constexpr FOCUS_EVENT_RECORD api(const FocusEventRecord& fer)
    {
        return FOCUS_EVENT_RECORD{ api(fer.setFocus) };
    }

    constexpr INPUT_RECORD api(const InputRecord& ir)
    {
        INPUT_RECORD record;
        record.EventType = api(ir.eventType);
        switch (ir.eventType)
        {
        case EventType::KeyEvent:
            record.Event.KeyEvent = api(ir.event.keyEvent);
            break;
        case EventType::MouseEvent:
            record.Event.MouseEvent = api(ir.event.mouseEvent);
            break;
        case EventType::WindowBufferSizeEvent:
            record.Event.WindowBufferSizeEvent = api(ir.event.windowBufferSizeEvent);
            break;
        case EventType::MenuEvent:
            record.Event.MenuEvent = api(ir.event.menuEvent);
            break;
        case EventType::FocusEvent:
            record.Event.FocusEvent = api(ir.event.focusEvent);
            break;
        }
        return record;
    }

    #pragma endregion
    #pragma endregion

    #pragma region wci
    #pragma region types

    constexpr Wchar wci(WCHAR wch)
    {
        return static_cast<Wchar>(wch);
    }

    constexpr Short wci(SHORT sh)
    {
        return static_cast<Short>(sh);
    }

    constexpr Word wci(WORD w)
    {
        return static_cast<Word>(w);
    }

    constexpr Dword wci(DWORD dw)
    {
        return static_cast<Dword>(dw);
    }

    constexpr Handle wci(HANDLE h)
    {
        return static_cast<Handle>(h);
    }

    constexpr bool wci(BOOL b)
    {
        return static_cast<bool>(b);
    }

    constexpr unsigned int wci(UINT ui)
    {
        return static_cast<unsigned int>(ui);
    }

    #pragma endregion

    #pragma region structs

    constexpr Coord wci(const COORD& c)
    {
        return Coord{ wci(c.X), wci(c.Y) };
    }

    constexpr SmallRect wci(const SMALL_RECT& sr)
    {
        return SmallRect{ wci(sr.Left), wci(sr.Top), wci(sr.Right), wci(sr.Bottom) };
    }

    constexpr CharInfo wci(const CHAR_INFO& chi)
    {
        return CharInfo{ wci(chi.Char.UnicodeChar), Attribute(chi.Attributes) };
    }

    constexpr CursorInfo wci(const CONSOLE_CURSOR_INFO& cci)
    {
        return CursorInfo{ wci(cci.dwSize), wci(cci.bVisible) };
    }

    constexpr ScreenBufferInfo wci(const CONSOLE_SCREEN_BUFFER_INFO& csbi)
    {
        return ScreenBufferInfo{
            wci(csbi.dwSize),
            wci(csbi.dwCursorPosition),
            Attribute(csbi.wAttributes),
            wci(csbi.srWindow),
            wci(csbi.dwMaximumWindowSize)
        };
    }

    constexpr KeyEventRecord wci(const KEY_EVENT_RECORD& ker)
    {
        return KeyEventRecord{
            wci(ker.bKeyDown),
            wci(ker.wRepeatCount),
            VirtualKey(ker.wVirtualKeyCode),
            VirtualKey(ker.wVirtualScanCode),
            wci(ker.uChar.UnicodeChar),
            ControlKeyState(ker.dwControlKeyState)
        };
    }

    constexpr MouseEventRecord wci(const MOUSE_EVENT_RECORD& mer)
    {
        return MouseEventRecord{
            wci(mer.dwMousePosition),
            ButtonState(mer.dwButtonState),
            ControlKeyState(mer.dwControlKeyState),
            EventFlag(mer.dwEventFlags)
        };
    }

    constexpr WindowBufferSizeRecord wci(const WINDOW_BUFFER_SIZE_RECORD& wbsr)
    {
        return WindowBufferSizeRecord{ wci(wbsr.dwSize) };
    }

    constexpr MenuEventRecord wci(const MENU_EVENT_RECORD& mer)
    {
        return MenuEventRecord{ wci(mer.dwCommandId) };
    }

    constexpr FocusEventRecord wci(const FOCUS_EVENT_RECORD& fer)
    {
        return FocusEventRecord{ wci(fer.bSetFocus) };
    }

    constexpr InputRecord wci(const INPUT_RECORD& ir)
    {
        InputRecord record;
        record.eventType = EventType(ir.EventType);
        switch (record.eventType)
        {
        case EventType::KeyEvent:
            record.event.keyEvent = wci(ir.Event.KeyEvent);
            break;
        case EventType::MouseEvent:
            record.event.mouseEvent = wci(ir.Event.MouseEvent);
            break;
        case EventType::WindowBufferSizeEvent:
            record.event.windowBufferSizeEvent = wci(ir.Event.WindowBufferSizeEvent);
            break;
        case EventType::MenuEvent:
            record.event.menuEvent = wci(ir.Event.MenuEvent);
            break;
        case EventType::FocusEvent:
            record.event.focusEvent = wci(ir.Event.FocusEvent);
            break;
        }
        return record;
    }

    #pragma endregion
    #pragma endregion
}

#endif  // CINTER_INCLUDE_WINAPI_HPP