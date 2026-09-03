#pragma once
#ifndef CINTER_INCLUDE_DEFINITIONS_STRUCTS_HPP
#define CINTER_INCLUDE_DEFINITIONS_STRUCTS_HPP

#include "types.hpp"

namespace wci
{
    struct Coord
    {
        Short x;
        Short y;
    };

    struct SmallRect
    {
        Short left;    // X of top left corner.
        Short top;     // Y of top left corner.
        Short right;   // X of bottom right corner.
        Short bottom;  // Y of bottom right corner.
    };

    struct CharInfo
    {
        wchar_t character;
        Word attributes;
    };

    struct CursorInfo
    {
        Dword size;
        bool visible;
    };

    struct ScreenBufferInfo
    {
        Coord size;
        Coord cursorPosition;
        Word attributes;
        SmallRect window;
        Coord maxWindowSize;
    };

    struct KeyEventRecord
    {
        bool keyDown;
        Word repeatCount;
        Word virtualKeyCode;
        Word virtualScanCode;
        wchar_t character;
        Dword controlKeyState;
    };

    struct MouseEventRecord
    {
        Coord mousePosition;
        Dword buttonState;
        Dword controlKeyState;
        Dword eventFlags;
    };

    struct WindowBufferSizeRecord
    {
        Coord size;
    };

    struct MenuEventRecord
    {
        unsigned int commandId;
    };

    struct FocusEventRecord
    {
        bool setFocus;
    };

    struct InputRecord
    {
        Word eventType;
        union {
            KeyEventRecord keyEvent;
            MouseEventRecord mouseEvent;
            WindowBufferSizeRecord windowBufferSizeEvent;
            MenuEventRecord menuEvent;
            FocusEventRecord focusEvent;
        } event;
    };
}

#endif  // CINTER_INCLUDE_DEFINITIONS_STRUCTS_HPP
