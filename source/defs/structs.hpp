#pragma once
#ifndef WCI_SOURCE_DEFS_STRUCTS_HPP
#define WCI_SOURCE_DEFS_STRUCTS_HPP

#include "enums.hpp"
#include "types.hpp"

#include "charcell.hpp"
#include "geometry.hpp"
#include "point.hpp"
#include "positionspec.hpp"
#include "rect.hpp"

namespace wci
{
    #pragma region Console Info

    /*
     * Structure `CursorInfo` contains information about the console cursor.
     */
    struct CursorInfo
    {
        Dword size;
        bool visible;

        constexpr bool operator==(const CursorInfo& other) const noexcept;

        constexpr bool operator!=(const CursorInfo& other) const noexcept;
    };

    /*
     * Structure `ScreenBufferInfo` contains information about the console screen buffer.
     */
    struct ScreenBufferInfo
    {
        Geometry size;
        Point cursorPosition;
        Attribute attributes;
        Rect window;
        Geometry maxWindowSize;

        constexpr bool operator==(const ScreenBufferInfo& other) const noexcept;

        constexpr bool operator!=(const ScreenBufferInfo& other) const noexcept;
    };

    #pragma endregion

    #pragma region Events

    /*
     * Structure `KeyEvent` is used for recording keyboard input events for structure `InputEvent`.
     */
    struct KeyEvent
    {
        bool keyDown;
        Word repeatCount;
        VirtualKey virtualKeyCode;
        VirtualKey virtualScanCode;
        Wchar character;
        ControlKeyState controlKeyState;
    };

    /*
     * Structure `MouseEvent` is used for recording mouse input events for structure `InputEvent`.
     */
    struct MouseEvent
    {
        Point mousePosition;
        ButtonState buttonState;
        ControlKeyState controlKeyState;
        EventFlag eventFlags;
    };

    /*
     * Structure `WindowBufferResizeEvent` is used for recording events of
     * changing the size of window buffer for structure `InputEvent`.
     */
    struct WindowBufferResizeEvent
    {
        Geometry size;
    };

    /*
     * Structure `MenuEvent` is used for recording menu events for structure `InputEvent`.
     * Such events are for internal use and should be ignored.
     */
    struct MenuEvent
    {
        unsigned int commandId;
    };

    /*
     * Structure `FocusEvent` is used for recording focus events for structure `InputEvent`.
     * Such events are for internal use and should be ignored.
     */
    struct FocusEvent
    {
        bool setFocus;
    };

    /*
     * Structure `InputEvent` is used for recording data input events for console input buffer.
     */
    struct InputEvent  // idea make variant
    {
        EventType eventType;
        union {
            KeyEvent keyEvent;
            MouseEvent mouseEvent;
            WindowBufferResizeEvent windowBufferSizeEvent;
            MenuEvent menuEvent;
            FocusEvent focusEvent;
        } event;
    };

    #pragma endregion

    struct Borders
    {
        BorderWidth left;
        BorderWidth top;
        BorderWidth right;
        BorderWidth bottom;
    };
}

inline constexpr bool wci::CursorInfo::operator==(const CursorInfo& other) const noexcept
{
    return size == other.size && visible == other.visible;
}

inline constexpr bool wci::CursorInfo::operator!=(const CursorInfo& other) const noexcept
{
    return !(*this == other);
}

inline constexpr bool wci::ScreenBufferInfo::operator==(const ScreenBufferInfo& other) const noexcept
{
    return
    (
        size ==           other.size           &&
        cursorPosition == other.cursorPosition &&
        attributes ==     other.attributes     &&
        window ==         other.window         &&
        maxWindowSize ==  other.maxWindowSize
    );
}

inline constexpr bool wci::ScreenBufferInfo::operator!=(const ScreenBufferInfo& other) const noexcept
{
    return !(*this == other);
}

#endif  // WCI_SOURCE_DEFS_STRUCTS_HPP
