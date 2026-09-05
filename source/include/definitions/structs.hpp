#pragma once
#ifndef CINTER_INCLUDE_DEFINITIONS_STRUCTS_HPP
#define CINTER_INCLUDE_DEFINITIONS_STRUCTS_HPP

#include "enums.hpp"
#include "types.hpp"

namespace wci
{
    /*
     * Structure `Coord` defines the coordinates of a character cell in  the console screen buffer.
     * The origin of the coordinate system (0; 0) is the upper-left cell of the buffer.
     */
    struct Coord
    {
        Short x;
        Short y;
    };

    /*
     * Structure `SmallRect` defines coordinates of upper-left and lower-right corners of a rectangle.
     */
    struct SmallRect
    {
        Short left;    // X of top left corner.
        Short top;     // Y of top left corner.
        Short right;   // X of bottom right corner.
        Short bottom;  // Y of bottom right corner.
    };

    /*
     * Structure `CharInfo` specifies a Unicode character and its attributes.
     * This structure is used by the console functions for reading from and writing to the console screen buffer.
     */
    struct CharInfo
    {
        wchar_t character;
        Attribute attributes;  // Word
    };

    #pragma region Console info structures

    /*
     * Structure `CursorInfo` contains information about the console cursor.
     */
    struct CursorInfo
    {
        Dword size;
        bool visible;
    };

    /*
     * Structure `ScreenBufferInfo` contains information about the console screen buffer.
     */
    struct ScreenBufferInfo
    {
        Coord size;
        Coord cursorPosition;
        Attribute attributes;  // Word
        SmallRect window;
        Coord maxWindowSize;
    };

    #pragma endregion

    #pragma region Event records

    /*
     * Structure `KeyEventRecord` is used for recording keyboard input events for structure `InputRecord`.
     */
    struct KeyEventRecord
    {
        bool keyDown;
        Word repeatCount;
        VirtualKey virtualKeyCode;  // Word
        VirtualKey virtualScanCode;  // Word
        wchar_t character;
        ControlKeyState controlKeyState;  // Dword
    };

    /*
     * Structure `MouseEventRecord` is used for recording mouse input events for structure `InputRecord`.
     */
    struct MouseEventRecord
    {
        Coord mousePosition;
        ButtonState buttonState;  // Dword
        ControlKeyState controlKeyState;  // Dword
        EventFlag eventFlags;  // Dword
    };

    /*
     * Structure `WindowBufferSizeRecord` is used for recording events of
     * changing the size of window buffer for structure `InputRecord`.
     */
    struct WindowBufferSizeRecord
    {
        Coord size;
    };

    /*
     * Structure `MenuEventRecord` is used for recording menu events for structure `InputRecord`.
     * Such events are for internal use and should be ignored.
     */
    struct MenuEventRecord
    {
        unsigned int commandId;
    };

    /*
     * Structure `FocusEventRecord` is used for recording focus events for structure `InputRecord`.
     * Such events are for internal use and should be ignored.
     */
    struct FocusEventRecord
    {
        bool setFocus;
    };

    #pragma endregion

    /*
     * Structure `InputRecord` is used for recording data input events for console input buffer.
     */
    struct InputRecord
    {
        EventType eventType;  // Word
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
