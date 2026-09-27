#pragma once
#ifndef CINTER_INCLUDE_DEFINITIONS_STRUCTS_HPP
#define CINTER_INCLUDE_DEFINITIONS_STRUCTS_HPP

#include "enums.hpp"
#include "lenpercent.hpp"
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

    constexpr bool operator==(const Coord& a, const Coord& b) noexcept
    {
        return a.x == b.x && a.y == b.y;
    }

    constexpr bool operator!=(const Coord& a, const Coord& b) noexcept
    {
        return a.x != b.x || a.y != b.y;
    }

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

    constexpr bool operator==(const SmallRect& a, const SmallRect& b) noexcept
    {
        return (
            a.left == b.left &&
            a.top == b.top &&
            a.right == b.right &&
            a.bottom == b.bottom
        );
    }

    constexpr bool operator!=(const SmallRect& a, const SmallRect& b) noexcept
    {
        return (
            a.left != b.left ||
            a.top != b.top ||
            a.right != b.right ||
            a.bottom != b.bottom
        );
    }

    /*
     * Structure `CharInfo` specifies a Unicode character and its attributes.
     * This structure is used by the console functions for reading from and writing to the console screen buffer.
     */
    struct CharInfo
    {
        Wchar character;
        Attribute attributes;  // Word
    };

    constexpr bool operator==(const CharInfo& a, const CharInfo& b) noexcept
    {
        return a.character == b.character && a.attributes == b.attributes;
    }

    constexpr bool operator!=(const CharInfo& a, const CharInfo& b) noexcept
    {
        return a.character != b.character || a.attributes != b.attributes;
    }

    #pragma region Console info structures

    /*
     * Structure `CursorInfo` contains information about the console cursor.
     */
    struct CursorInfo
    {
        Dword size;
        bool visible;
    };

    constexpr bool operator==(const CursorInfo& a, const CursorInfo& b) noexcept
    {
        return a.size == b.size && a.visible == b.visible;
    }

    constexpr bool operator!=(const CursorInfo& a, const CursorInfo& b) noexcept
    {
        return a.size != b.size || a.visible != b.visible;
    }

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

    constexpr bool operator==(const ScreenBufferInfo& a, const ScreenBufferInfo& b) noexcept
    {
        return (
            a.size == b.size &&
            a.cursorPosition == b.cursorPosition &&
            a.attributes == b.attributes &&
            a.window == b.window &&
            a.maxWindowSize == b.maxWindowSize
        );
    }

    constexpr bool operator!=(const ScreenBufferInfo& a, const ScreenBufferInfo& b) noexcept
    {
        return (
            a.size != b.size ||
            a.cursorPosition != b.cursorPosition ||
            a.attributes != b.attributes ||
            a.window != b.window ||
            a.maxWindowSize != b.maxWindowSize
        );
    }

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
        Wchar character;
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

    #pragma region PositionSpec

    struct PositionSpec
    {
        LenPercent x;
        LenPercent y;
    };

    inline constexpr PositionSpec& operator+=(PositionSpec& ps, const PositionSpec& other)
    {
        ps.x += other.x;
        ps.y += other.y;
        return ps;
    }
    inline constexpr PositionSpec& operator+=(PositionSpec& ps, int other)
    {
        ps.x += other;
        ps.y += other;
        return ps;
    }

    inline constexpr PositionSpec& operator-=(PositionSpec& ps, const PositionSpec& other)
    {
        ps.x -= other.x;
        ps.y -= other.y;
        return ps;
    }
    inline constexpr PositionSpec& operator-=(PositionSpec& ps, int other)
    {
        ps.x -= other;
        ps.y -= other;
        return ps;
    }

    inline constexpr PositionSpec& operator*=(PositionSpec& ps, int other) noexcept
    {
        ps.x *= other;
        ps.y *= other;
        return ps;
    }

    inline constexpr PositionSpec& operator/=(PositionSpec& ps, int other)
    {
        ps.x /= other;
        ps.y /= other;
        return ps;
    }

    inline constexpr PositionSpec& operator%=(PositionSpec& ps, int other)
    {
        ps.x %= other;
        ps.y %= other;
        return ps;
    }

    inline constexpr PositionSpec operator+(const PositionSpec& ps, const PositionSpec& other)
    {
        auto result = ps;
        return result += other;
    }
    inline constexpr PositionSpec operator+(const PositionSpec& ps, int other)
    {
        auto result = ps;
        return result += other;
    }

    inline constexpr PositionSpec operator-(const PositionSpec& ps, const PositionSpec& other)
    {
        auto result = ps;
        return result -= other;
    }
    inline constexpr PositionSpec operator-(const PositionSpec& ps, int other)
    {
        auto result = ps;
        return result -= other;
    }
    
    inline constexpr PositionSpec operator*(const PositionSpec& ps, int other) noexcept
    {
        auto result = ps;
        return result *= other;
    }
    
    inline constexpr PositionSpec operator/(const PositionSpec& ps, int other)
    {
        auto result = ps;
        return result /= other;
    }
    
    inline constexpr PositionSpec operator%(const PositionSpec& ps, int other)
    {
        auto result = ps;
        return result %= other;
    }

    inline constexpr PositionSpec operator+(const PositionSpec& ps) noexcept
    {
        return ps;
    }

    inline constexpr PositionSpec operator-(const PositionSpec& ps)
    {
        return PositionSpec{ -ps.x, -ps.y };
    }

    inline constexpr bool operator==(const PositionSpec& ps, const PositionSpec& other) noexcept
    {
        return ps.x == other.x && ps.y == other.y;
    }

    inline constexpr bool operator!=(const PositionSpec& ps, const PositionSpec& other) noexcept
    {
        return !(ps == other);
    }

    inline constexpr PositionSpec& operator++(PositionSpec& ps)
    {
        ++ps.x;
        ++ps.y;
        return ps;
    }

    inline constexpr PositionSpec& operator--(PositionSpec& ps)
    {
        --ps.x;
        --ps.y;
        return ps;
    }

    inline constexpr PositionSpec operator++(PositionSpec& ps, int)
    {
        auto old = ps;
        ++ps;
        return old;
    }

    inline constexpr PositionSpec operator--(PositionSpec& ps, int)
    {
        auto old = ps;
        --ps;
        return old;
    }

    #pragma endregion

    struct Geometry
    {
        Short width;
        Short height;
    };

    constexpr bool operator==(const Geometry& a, const Geometry& b) noexcept
    {
        return a.width == b.width && a.height == b.height;
    }

    constexpr bool operator!=(const Geometry& a, const Geometry& b) noexcept
    {
        return a.width != b.width || a.height != b.height;
    }

    struct FixedGeometry  // fixme reimplement
    {
        bool width;
        bool height;
    };

    struct Borders
    {
        BorderWidth left;
        BorderWidth top;
        BorderWidth right;
        BorderWidth bottom;
    };
}

#endif  // CINTER_INCLUDE_DEFINITIONS_STRUCTS_HPP
