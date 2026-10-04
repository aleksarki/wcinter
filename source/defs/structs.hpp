#pragma once
#ifndef CINTER_INCLUDE_DEFINITIONS_STRUCTS_HPP
#define CINTER_INCLUDE_DEFINITIONS_STRUCTS_HPP

#include "enums.hpp"
#include "lenpercent.hpp"
#include "types.hpp"

namespace wci
{
    /*
     * Structure `Point` defines the coordinates of a character cell in  the console screen buffer.
     * The origin of the coordinate system (0; 0) is the upper-left cell of the buffer.
     */
    struct Point
    {
        Short x;
        Short y;

        constexpr bool operator==(const Point& other) const noexcept;

        constexpr bool operator!=(const Point& other) const noexcept;
    };

    /*
     * Structure `Rect` defines coordinates of upper-left and lower-right corners of a rectangle.
     */
    struct Rect
    {
        Short left;    // X of top left corner.
        Short top;     // Y of top left corner.
        Short right;   // X of bottom right corner.
        Short bottom;  // Y of bottom right corner.

        constexpr bool operator==(const Rect& other) const noexcept;

        constexpr bool operator!=(const Rect& other) const noexcept;
    };

    /*
     * Structure `CharCell` specifies a Unicode character and its attributes.
     * This structure is used by the console functions for reading from and writing to the console screen buffer.
     */
    struct CharCell
    {
        Wchar character;
        Attribute attributes;

        constexpr bool operator==(const CharCell& other) const noexcept;

        constexpr bool operator!=(const CharCell& other) const noexcept;
    };

    #pragma region Console info structures

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
        Point size;
        Point cursorPosition;
        Attribute attributes;
        Rect window;
        Point maxWindowSize;

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
        Point size;
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

    #pragma endregion

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

    struct PositionSpec
    {
        LenPercent x;
        LenPercent y;

        constexpr PositionSpec& operator+=(const PositionSpec& other);
        constexpr PositionSpec& operator+=(int other);

        constexpr PositionSpec& operator-=(const PositionSpec& other);
        constexpr PositionSpec& operator-=(int other);

        constexpr PositionSpec& operator*=(int other) noexcept;

        constexpr PositionSpec& operator/=(int other);

        constexpr PositionSpec& operator%=(int other);

        constexpr PositionSpec operator+(const PositionSpec& other) const;
        constexpr PositionSpec operator+(int other) const;

        constexpr PositionSpec operator-(const PositionSpec& other) const;
        constexpr PositionSpec operator-(int other) const;

        constexpr PositionSpec operator*(int other) const noexcept;

        constexpr PositionSpec operator/(int other) const;

        constexpr PositionSpec operator%(int other) const;

        constexpr PositionSpec operator+() const noexcept;

        constexpr PositionSpec operator-() const;

        constexpr bool operator==(const PositionSpec& other) const noexcept;

        constexpr bool operator!=(const PositionSpec& other) const noexcept;

        constexpr PositionSpec& operator++();

        constexpr PositionSpec& operator--();

        constexpr PositionSpec operator++(int);

        constexpr PositionSpec operator--(int);
    };

    struct Geometry
    {
        Short width;
        Short height;

        constexpr bool operator==(const Geometry& other) const noexcept;
        constexpr bool operator!=(const Geometry& other) const noexcept;
    };

    struct Borders
    {
        BorderWidth left;
        BorderWidth top;
        BorderWidth right;
        BorderWidth bottom;
    };
}

inline constexpr bool wci::Point::operator==(const wci::Point& other) const noexcept
{
    return x == other.x && y == other.y;
}

inline constexpr bool wci::Point::operator!=(const wci::Point& other) const noexcept
{
    return !(*this == other);
}

inline constexpr bool wci::Rect::operator==(const wci::Rect& other) const noexcept
{
    return
    (
        left ==   other.left  &&
        top ==    other.top   &&
        right ==  other.right &&
        bottom == other.bottom
    );
}

inline constexpr bool wci::Rect::operator!=(const wci::Rect& other) const noexcept
{
    return !(*this == other);
}

inline constexpr bool wci::CharCell::operator==(const wci::CharCell& other) const noexcept
{
    return character == other.character && attributes == other.attributes;
}

inline constexpr bool wci::CharCell::operator!=(const wci::CharCell& other) const noexcept
{
    return !(*this == other);
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

inline constexpr wci::PositionSpec& wci::PositionSpec::operator+=(const wci::PositionSpec& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

inline constexpr wci::PositionSpec& wci::PositionSpec::operator+=(int other)
{
    x += other;
    y += other;
    return *this;
}
inline constexpr wci::PositionSpec& wci::PositionSpec::operator-=(const wci::PositionSpec& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

inline constexpr wci::PositionSpec& wci::PositionSpec::operator-=(int other)
{
    x -= other;
    y -= other;
    return *this;
}

inline constexpr wci::PositionSpec& wci::PositionSpec::operator*=(int other) noexcept
{
    x *= other;
    y *= other;
    return *this;
}

inline constexpr wci::PositionSpec& wci::PositionSpec::operator/=(int other)
{
    x /= other;
    y /= other;
    return *this;
}

inline constexpr wci::PositionSpec& wci::PositionSpec::operator%=(int other)
{
    x %= other;
    y %= other;
    return *this;
}

inline constexpr wci::PositionSpec wci::PositionSpec::operator+(const wci::PositionSpec& other) const
{
    auto result = *this;
    return result += other;
}
inline constexpr wci::PositionSpec wci::PositionSpec::operator+(int other) const
{
    auto result = *this;
    return result += other;
}

inline constexpr wci::PositionSpec wci::PositionSpec::operator-(const wci::PositionSpec& other) const
{
    auto result = *this;
    return result -= other;
}
inline constexpr wci::PositionSpec wci::PositionSpec::operator-(int other) const
{
    auto result = *this;
    return result -= other;
}

inline constexpr wci::PositionSpec wci::PositionSpec::operator*(int other) const noexcept
{
    auto result = *this;
    return result *= other;
}

inline constexpr wci::PositionSpec wci::PositionSpec::operator/(int other) const
{
    auto result = *this;
    return result /= other;
}

inline constexpr wci::PositionSpec wci::PositionSpec::operator%(int other) const
{
    auto result = *this;
    return result %= other;
}

inline constexpr wci::PositionSpec wci::PositionSpec::operator+() const noexcept
{
    return *this;
}

inline constexpr wci::PositionSpec wci::PositionSpec::operator-() const
{
    return wci::PositionSpec{ -x, -y };
}

inline constexpr bool wci::PositionSpec::operator==(const wci::PositionSpec& other) const noexcept
{
    return x == other.x && y == other.y;
}

inline constexpr bool wci::PositionSpec::operator!=(const wci::PositionSpec& other) const noexcept
{
    return !(*this == other);
}

inline constexpr wci::PositionSpec& wci::PositionSpec::operator++()
{
    ++x;
    ++y;
    return *this;
}

inline constexpr wci::PositionSpec& wci::PositionSpec::operator--()
{
    --x;
    --y;
    return *this;
}

inline constexpr wci::PositionSpec wci::PositionSpec::operator++(int)
{
    auto old = *this;
    ++*this;
    return old;
}

inline constexpr wci::PositionSpec wci::PositionSpec::operator--(int)
{
    auto old = *this;
    --*this;
    return old;
}

inline constexpr bool wci::Geometry::operator==(const wci::Geometry& other) const noexcept
{
    return width == other.width && height == other.height;
}

inline constexpr bool wci::Geometry::operator!=(const wci::Geometry& other) const noexcept
{
    return !(*this == other);
}

#endif  // CINTER_INCLUDE_DEFINITIONS_STRUCTS_HPP
