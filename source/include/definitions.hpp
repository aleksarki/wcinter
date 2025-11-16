#pragma once
#ifndef CINTER_INCLUDE_STRUCTS_HPP
#define CINTER_INCLUDE_STRUCTS_HPP

#include <vector>

namespace cinter {

    using Short = short;
    using Word = unsigned short;
    using Dword = unsigned long;
    using Handle = void*;

    enum class Attribute
    {
        ForegroundBlue =      0x0001,  // Text color contains blue.
        ForegroundGreen =     0x0002,  // Text color contains green.
        ForegroundRed =       0x0004,  // Text color contains red.
        ForegroundIntensity = 0x0008,  // Text color is intensified.
        
        BackgroundBlue =      0x0010,  // Background color contains blue.
        BackgroundGreen =     0x0020,  // Background color contains green.
        BackgroundRed =       0x0040,  // Background color contains red.
        BackgroundIntensity = 0x0080,  // Background color is intensified.

        CommonLvbLeadingByte =    0x0100,  // Leading Byte of DBCS.
        CommonLvbTrailingByte =   0x0200,  // Trailing Byte of DBCS.
        CommonLvbGridHorizontal = 0x0400,  // DBCS: Grid attribute: top horizontal.
        CommonLvbGridLVertical =  0x0800,  // DBCS: Grid attribute: left vertical.
        CommonLvbGridRVertical =  0x1000,  // DBCS: Grid attribute: right vertical.
        CommonLvbReverseVideo =   0x4000,  // DBCS: Reverse fore/back ground attribute.
        CommonLvbUnderscore =     0x8000,  // DBCS: Underscore.
        CommonLvbSbcsDbcs =       0x0300,  // SBCS or DBCS flag.

        FgColorBlack =   0,                                                  // Text color is black.
        FgColorBlue =    ForegroundBlue,                                     // Text color is blue.
        FgColorGreen =   ForegroundGreen,                                    // Text color is green.
        FgColorCyan =    ForegroundBlue  | ForegroundGreen,                  // Text color is cyan.
        FgColorRed =     ForegroundRed,                                      // Text color is red.
        FgColorMagenta = ForegroundBlue  | ForegroundRed,                    // Text color is magenta.
        FgColorYellow =  ForegroundGreen | ForegroundRed,                    // Text color is yellow.
        FgColorWhite =   ForegroundBlue  | ForegroundGreen | ForegroundRed,  // Text color is white.

        FgColorBlackBright =   FgColorBlack   | ForegroundIntensity,  // Text color is bright black.
        FgColorBlueBright =    FgColorBlue    | ForegroundIntensity,  // Text color is bright blue.
        FgColorGreenBright =   FgColorGreen   | ForegroundIntensity,  // Text color is bright green.
        FgColorCyanBright =    FgColorCyan    | ForegroundIntensity,  // Text color is bright cyan.
        FgColorRedBright =     FgColorRed     | ForegroundIntensity,  // Text color is bright red.
        FgColorMagentaBright = FgColorMagenta | ForegroundIntensity,  // Text color is bright magenta.
        FgColorYellowBright =  FgColorYellow  | ForegroundIntensity,  // Text color is bright yellow.
        FgColorWhiteBright =   FgColorWhite   | ForegroundIntensity,  // Text color is bright white.

        BgColorBlack =   0,                                                  // Background color is black.
        BgColorBlue =    BackgroundBlue,                                     // Background color is blue.
        BgColorGreen =   BackgroundGreen,                                    // Background color is green.
        BgColorCyan =    BackgroundBlue  | BackgroundGreen,                  // Background color is cyan.
        BgColorRed =     BackgroundRed,                                      // Background color is red.
        BgColorMagenta = BackgroundBlue  | BackgroundRed,                    // Background color is magenta.
        BgColorYellow =  BackgroundGreen | BackgroundRed,                    // Background color is yellow.
        BgColorWhite =   BackgroundBlue  | BackgroundGreen | BackgroundRed,  // Background color is white.

        BgColorBlackBright =   BgColorBlack   | BackgroundIntensity,  // Background color is bright black.
        BgColorBlueBright =    BgColorBlue    | BackgroundIntensity,  // Background color is bright blue.
        BgColorGreenBright =   BgColorGreen   | BackgroundIntensity,  // Background color is bright green.
        BgColorCyanBright =    BgColorCyan    | BackgroundIntensity,  // Background color is bright cyan.
        BgColorRedBright =     BgColorRed     | BackgroundIntensity,  // Background color is bright red.
        BgColorMagentaBright = BgColorMagenta | BackgroundIntensity,  // Background color is bright magenta.
        BgColorYellowBright =  BgColorYellow  | BackgroundIntensity,  // Background color is bright yellow.
        BgColorWhiteBright =   BgColorWhite   | BackgroundIntensity   // Background color is bright white.
    };

    enum class GenericRights : Dword
    {
        Read =    0x80000000L,
        Write =   0x40000000L,
        Execute = 0x20000000L,
        All =     0x10000000L
    };

    enum class FileAccessRights : Dword
    {
        ShareRead =   0x00000001,
        ShareWrite =  0x00000002,
        ShareDelete = 0x00000004
    };

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

    class CharMatrix
    {
    private:
        std::vector<CharInfo> mat;
        Coord siz;
    
    public:
        CharMatrix(Short x, Short y) : siz{ x, y }
        {
            mat.resize(siz.x * siz.y);
        }
        CharMatrix(Coord size) : siz(size)
        {
            mat.resize(size.x * size.y);
        }

        CharInfo& at(Short x, Short y) noexcept
        {
            return mat[y * siz.x + x];
        }
        const CharInfo& at(Short x, Short y) const noexcept
        {
            return mat[y * siz.x + x];
        }
        CharInfo& at(const Coord& position) noexcept
        {
            return mat[position.y * siz.x + position.x];
        }
        const CharInfo& at(const Coord& position) const noexcept
        {
            return mat[position.y * siz.x + position.x];
        }

        void put(Short x, Short y, CharInfo charInfo)
        {
            mat[y * siz.x + x] = charInfo;
        }
        void put(Short x, Short y, wchar_t character, Word attributes)
        {
            mat[y * siz.x + x] = CharInfo{ character, attributes };
        }
        void put(const Coord& position, CharInfo charInfo)
        {
            mat[position.y * siz.x + position.x] = charInfo;
        }
    
        CharInfo* data() noexcept
        {
            return mat.data();
        }
        const CharInfo* data() const noexcept
        {
            return mat.data();
        }
    
        Coord& size() noexcept
        {
            return siz;
        }
        const Coord& size() const noexcept
        {
            return siz;
        }
    };

}

#endif  // CINTER_INCLUDE_STRUCTS_HPP
