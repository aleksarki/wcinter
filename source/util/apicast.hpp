#pragma once
#ifndef WCI_SOURCE_UTIL_APICAST_HPP
#define WCI_SOURCE_UTIL_APICAST_HPP

#include <windows.h>
#include "../defs.hpp"

namespace wci
{
    #pragma region api
    #pragma region types

    inline constexpr WCHAR api(Wchar wch)
    {
        return static_cast<WCHAR>(wch);
    }

    inline constexpr SHORT api(Short sh)
    {
        return static_cast<SHORT>(sh);
    }

    inline constexpr WORD api(Word w)
    {
        return static_cast<WORD>(w);
    }

    inline constexpr DWORD api(Dword dw)
    {
        return static_cast<DWORD>(dw);
    }

    inline constexpr HANDLE api(Handle h)
    {
        return static_cast<HANDLE>(h);
    }

    inline constexpr BOOL api(bool b)
    {
        return static_cast<BOOL>(b);
    }

    inline constexpr UINT api(unsigned int ui)
    {
        return static_cast<UINT>(ui);
    }

    inline constexpr DWORD api(size_t s)
    {
        return static_cast<DWORD>(s);
    }

    inline CONST CHAR_INFO* api(const CharInfo* cchip)
    {
        return reinterpret_cast<CONST CHAR_INFO*>(cchip);  /* fixme this is not good */
    }

    inline LPCWSTR api(const Wchar* cwchp)
    {
        return reinterpret_cast<LPCWSTR>(cwchp);
    }

    inline LPWSTR api(Wchar* wchp)
    {
        return reinterpret_cast<LPWSTR>(wchp);
    }

    inline LPDWORD api(Dword* dwp)
    {
        return reinterpret_cast<DWORD*>(dwp);
    }
    #pragma endregion

    #pragma region enums

    inline constexpr DWORD api(StdHandle sh)
    {
        return static_cast<DWORD>(sh);
    }

    inline constexpr int api(InputMode im)
    {
        return static_cast<int>(im);
    }

    inline constexpr int api(CodePage cp)
    {
        return static_cast<int>(cp);
    }

    inline constexpr WORD api(Attribute a)
    {
        return static_cast<WORD>(a);
    }

    inline constexpr DWORD api(GenericRights gr)
    {
        return static_cast<DWORD>(gr);
    }

    inline constexpr DWORD api(FileAccessRights far_)
    {
        return static_cast<DWORD>(far_);
    }

    inline constexpr DWORD api(ControlKeyState cks)
    {
        return static_cast<DWORD>(cks);
    }

    inline constexpr DWORD api(ButtonState bs)
    {
        return static_cast<DWORD>(bs);
    }

    inline constexpr DWORD api(EventFlag ef)
    {
        return static_cast<DWORD>(ef);
    }

    inline constexpr WORD api(EventType et)
    {
        return static_cast<WORD>(et);
    }

    inline constexpr WORD api(VirtualKey vk)
    {
        return static_cast<WORD>(vk);
    }

    #pragma endregion

    #pragma region structs

    inline constexpr COORD api(const Coord& c)
    {
        return COORD{ api(c.x), api(c.y) };
    }

    inline constexpr SMALL_RECT api(const SmallRect& sr)
    {
        return SMALL_RECT{ api(sr.left), api(sr.top), api(sr.right), api(sr.bottom) };
    }

    inline constexpr CHAR_INFO api(const CharInfo& chi)
    {
        CHAR_INFO info;
        info.Char.UnicodeChar = api(chi.character);
        info.Attributes = api(chi.attributes);
        return info;
    }

    inline constexpr CONSOLE_CURSOR_INFO api(const CursorInfo& ci)
    {
        return CONSOLE_CURSOR_INFO{ api(ci.size), api(ci.visible) };
    }

    inline constexpr CONSOLE_SCREEN_BUFFER_INFO api(const ScreenBufferInfo& sbi)
    {
        return CONSOLE_SCREEN_BUFFER_INFO{
            api(sbi.size),
            api(sbi.cursorPosition),
            api(sbi.attributes),
            api(sbi.window),
            api(sbi.maxWindowSize)
        };
    }

    inline constexpr KEY_EVENT_RECORD api(const KeyEventRecord ker)
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

    inline constexpr MOUSE_EVENT_RECORD api(const MouseEventRecord& mer)
    {
        return MOUSE_EVENT_RECORD{
            api(mer.mousePosition),
            api(mer.buttonState),
            api(mer.controlKeyState),
            api(mer.eventFlags)
        };
    }

    inline constexpr WINDOW_BUFFER_SIZE_RECORD api(const WindowBufferSizeRecord& wbsr)
    {
        return WINDOW_BUFFER_SIZE_RECORD{ api(wbsr.size) };
    }

    inline constexpr MENU_EVENT_RECORD api(const MenuEventRecord& mer)
    {
        return MENU_EVENT_RECORD{ api(mer.commandId) };
    }

    inline constexpr FOCUS_EVENT_RECORD api(const FocusEventRecord& fer)
    {
        return FOCUS_EVENT_RECORD{ api(fer.setFocus) };
    }

    inline constexpr INPUT_RECORD api(const InputRecord& ir)
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

    inline constexpr Wchar wci(WCHAR wch)
    {
        return static_cast<Wchar>(wch);
    }

    inline constexpr Short wci(SHORT sh)
    {
        return static_cast<Short>(sh);
    }

    inline constexpr Word wci(WORD w)
    {
        return static_cast<Word>(w);
    }

    inline constexpr Dword wci(DWORD dw)
    {
        return static_cast<Dword>(dw);
    }

    inline constexpr Handle wci(HANDLE h)
    {
        return static_cast<Handle>(h);
    }

    inline constexpr bool wci(BOOL b)
    {
        return static_cast<bool>(b);
    }

    inline constexpr unsigned int wci(UINT ui)
    {
        return static_cast<unsigned int>(ui);
    }

    inline const CharInfo* wci(CONST CHAR_INFO* cchip)
    {
        return reinterpret_cast<const CharInfo*>(cchip);   /* fixme this is not good */
    }

    inline const Wchar* wci(CONST WCHAR* cwchp)
    {
        return reinterpret_cast<const Wchar*>(cwchp);
    }

    inline Wchar* wci(WCHAR* wchp)
    {
        return reinterpret_cast<Wchar*>(wchp);
    }

    inline Dword* wci(LPDWORD dwp)
    {
        return reinterpret_cast<Dword*>(dwp);
    }
    #pragma endregion

    #pragma region structs

    inline constexpr Coord wci(const COORD& c)
    {
        return Coord{ wci(c.X), wci(c.Y) };
    }

    inline constexpr SmallRect wci(const SMALL_RECT& sr)
    {
        return SmallRect{ wci(sr.Left), wci(sr.Top), wci(sr.Right), wci(sr.Bottom) };
    }

    inline constexpr CharInfo wci(const CHAR_INFO& chi)
    {
        return CharInfo{ wci(chi.Char.UnicodeChar), Attribute(chi.Attributes) };
    }

    inline constexpr CursorInfo wci(const CONSOLE_CURSOR_INFO& cci)
    {
        return CursorInfo{ wci(cci.dwSize), wci(cci.bVisible) };
    }

    inline constexpr ScreenBufferInfo wci(const CONSOLE_SCREEN_BUFFER_INFO& csbi)
    {
        return ScreenBufferInfo{
            wci(csbi.dwSize),
            wci(csbi.dwCursorPosition),
            Attribute(csbi.wAttributes),
            wci(csbi.srWindow),
            wci(csbi.dwMaximumWindowSize)
        };
    }

    inline constexpr KeyEventRecord wci(const KEY_EVENT_RECORD& ker)
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

    inline constexpr MouseEventRecord wci(const MOUSE_EVENT_RECORD& mer)
    {
        return MouseEventRecord{
            wci(mer.dwMousePosition),
            ButtonState(mer.dwButtonState),
            ControlKeyState(mer.dwControlKeyState),
            EventFlag(mer.dwEventFlags)
        };
    }

    inline constexpr WindowBufferSizeRecord wci(const WINDOW_BUFFER_SIZE_RECORD& wbsr)
    {
        return WindowBufferSizeRecord{ wci(wbsr.dwSize) };
    }

    inline constexpr MenuEventRecord wci(const MENU_EVENT_RECORD& mer)
    {
        return MenuEventRecord{ wci(mer.dwCommandId) };
    }

    inline constexpr FocusEventRecord wci(const FOCUS_EVENT_RECORD& fer)
    {
        return FocusEventRecord{ wci(fer.bSetFocus) };
    }

    inline constexpr InputRecord wci(const INPUT_RECORD& ir)
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

#endif  // WCI_SOURCE_UTIL_APICAST_HPP
