#pragma once
#ifndef CINTER_INCLUDE_STRUCTS_HPP
#define CINTER_INCLUDE_STRUCTS_HPP

#include <vector>

namespace cinter {

    using Short = short;
    using Word = unsigned short;
    using Dword = unsigned long;
    using Handle = void*;

    enum class Attribute : Word
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

    enum class ControlKeyState : Dword
    {
        RightAltPressed =  0x00000001,  // the right alt key is pressed.
        LeftAltPressed =   0x00000002,  // the left alt key is pressed.
        RightCtrlPressed = 0x00000004,  // the right ctrl key is pressed.
        LeftCtrlPressed =  0x00000008,  // the left ctrl key is pressed.
        ShiftPressed =     0x00000010,  // the shift key is pressed.
        NumLockOn =        0x00000020,  // the numlock light is on.
        ScrollLockOn =     0x00000040,  // the scrolllock light is on.
        CapsLockOn =       0x00000080,  // the capslock light is on.
        EnhancedKey =      0x00000100,  // the key is enhanced.
        NlsDbcsChar =      0x00010000,  // DBCS for JPN: SBCS/DBCS mode.
        NlsAlphaNumeric =  0x00000000,  // DBCS for JPN: Alphanumeric mode.
        NlsKatakana =      0x00020000,  // DBCS for JPN: Katakana mode.
        NlsHiragana =      0x00040000,  // DBCS for JPN: Hiragana mode.
        NlsRoman =         0x00400000,  // DBCS for JPN: Roman/Noroman mode.
        NlsImeConversion = 0x00800000,  // DBCS for JPN: IME conversion.
        AltNumpadBit =     0x04000000,  // AltNumpad OEM char (copied from ntuser\inc\kbd.h) ;internal_NT
        NlsImeDisable =    0x20000000   // DBCS for JPN: IME enable/disable.
    };

    enum class ButtonState : Dword
    {
        FromLeft1stButtonPressed = 0x0001,
        RightmostButtonPressed   = 0x0002,
        FromLeft2ndButtonPressed = 0x0004,
        FromLeft3rdButtonPressed = 0x0008,
        FromLeft4thButtonPressed = 0x0010
    };

    enum class EventFlag : Dword
    {
        MouseMoved =    0x0001,
        DoubleClick =   0x0002,
        MouseWheeled =  0x0004,
        MouseHWheeled = 0x0008
    };

    enum class EventType : Word
    {
        KeyEvent =              0x0001,  // Event contains key event record
        MouseEvent =            0x0002,  // Event contains mouse event record
        WindowBufferSizeEvent = 0x0004,  // Event contains window change event record
        MenuEvent =             0x0008,  // Event contains menu event record
        FocusEvent =            0x0010   // Event contains focus change
    };

    enum class VirtualKey : Word
    {
        LButton =                      0x01,
        RButton =                      0x02,
        Cancel =                       0x03,
        MButton =                      0x04,  // NOT contiguous with L & RBUTTON
        XButton1 =                     0x05,  // NOT contiguous with L & RBUTTON
        XButton2 =                     0x06,  // NOT contiguous with L & RBUTTON
        Back =                         0x08,
        Tab =                          0x09,
        Clear =                        0x0C,
        Return =                       0x0D,
        Shift =                        0x10,
        Control =                      0x11,
        Menu =                         0x12,
        Pause =                        0x13,
        Capital =                      0x14,
        Kana =                         0x15,
        Hangul =                       0x15,
        ImeOn =                        0x16,
        Junja =                        0x17,
        Final =                        0x18,
        Hanja =                        0x19,
        Kanji =                        0x19,
        ImeOff =                       0x1A,
        Escape =                       0x1B,
        Convert =                      0x1C,
        NonConvert =                   0x1D,
        Accept =                       0x1E,
        ModeChange =                   0x1F,
        Space =                        0x20,
        Prior =                        0x21,
        Next =                         0x22,
        End =                          0x23,
        Home =                         0x24,
        Left =                         0x25,
        Up =                           0x26,
        Right =                        0x27,
        Down =                         0x28,
        Select =                       0x29,
        Print =                        0x2A,
        Execute =                      0x2B,
        Snapshot =                     0x2C,
        Insert =                       0x2D,
        Delete =                       0x2E,
        Help =                         0x2F,
        /*
        * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
        * 0x3A - 0x40 : unassigned
        * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
        */
        LWin =                         0x5B,
        RWin =                         0x5C,
        Apps =                         0x5D,
        Sleep =                        0x5F,
        Numpad0 =                      0x60,
        Numpad1 =                      0x61,
        Numpad2 =                      0x62,
        Numpad3 =                      0x63,
        Numpad4 =                      0x64,
        Numpad5 =                      0x65,
        Numpad6 =                      0x66,
        Numpad7 =                      0x67,
        Numpad8 =                      0x68,
        Numpad9 =                      0x69,
        Multiply =                     0x6A,
        Add =                          0x6B,
        Separator =                    0x6C,
        Subtract =                     0x6D,
        Decimal =                      0x6E,
        Divide =                       0x6F,
        F1 =                           0x70,
        F2 =                           0x71,
        F3 =                           0x72,
        F4 =                           0x73,
        F5 =                           0x74,
        F6 =                           0x75,
        F7 =                           0x76,
        F8 =                           0x77,
        F9 =                           0x78,
        F10 =                          0x79,
        F11 =                          0x7A,
        F12 =                          0x7B,
        F13 =                          0x7C,
        F14 =                          0x7D,
        F15 =                          0x7E,
        F16 =                          0x7F,
        F17 =                          0x80,
        F18 =                          0x81,
        F19 =                          0x82,
        F20 =                          0x83,
        F21 =                          0x84,
        F22 =                          0x85,
        F23 =                          0x86,
        F24 =                          0x87,
        NavigationView =               0x88,  // reserved
        NavigationMenu =               0x89,  // reserved
        NavigationUp =                 0x8A,  // reserved
        NavigationDown =               0x8B,  // reserved
        NavigationLeft =               0x8C,  // reserved
        NavigationRight =              0x8D,  // reserved
        NavigationAccept =             0x8E,  // reserved
        NavigationCancel =             0x8F,  // reserved
        NumLock =                      0x90,
        Scroll =                       0x91,
        OemNecEqual =                  0x92,  // '=' key on numpad
        OemFjJisho =                   0x92,  // 'Dictionary' key
        OemFjMasshou =                 0x93,  // 'Unregister word' key
        OemFjTouroku =                 0x94,  // 'Register word' key
        OemFjLOya =                    0x95,  // 'Left OYAYUBI' key
        OemFjROya =                    0x96,  // 'Right OYAYUBI' key
        LShift =                       0xA0,
        RShift =                       0xA1,
        LControl =                     0xA2,
        RControl =                     0xA3,
        LMenu =                        0xA4,
        RMenu =                        0xA5,
        BrowserBack =                  0xA6,
        BrowserForward =               0xA7,
        BrowserRefresh =               0xA8,
        BrowserStop =                  0xA9,
        BrowserSearch =                0xAA,
        BrowserFavorites =             0xAB,
        BrowserHome =                  0xAC,
        VolumeMute =                   0xAD,
        VolumeDown =                   0xAE,
        VolumeUp =                     0xAF,
        MediaNextTrack =               0xB0,
        MediaPrevTrack =               0xB1,
        MediaStop =                    0xB2,
        MediaPlayPause =               0xB3,
        LaunchMail =                   0xB4,
        LaunchMediaSelect =            0xB5,
        LaunchApp1 =                   0xB6,
        LaunchApp2 =                   0xB7,
        Oem1 =                         0xBA,  // ';:' for US
        OemPlus =                      0xBB,  // '+' any country
        OemComma =                     0xBC,  // ',' any country
        OemMinus =                     0xBD,  // '-' any country
        OemPeriod =                    0xBE,  // '.' any country
        Oem2 =                         0xBF,  // '/?' for US
        Oem3 =                         0xC0,  // '`~' for US
        GamepadA =                     0xC3,  // reserved
        GamepadB =                     0xC4,  // reserved
        GamepadX =                     0xC5,  // reserved
        GamepadY =                     0xC6,  // reserved
        GamepadRightShoulder =         0xC7,  // reserved  
        GamepadLeftShoulder =          0xC8,  // reserved  
        GamepadLeftTrigger =           0xC9,  // reserved  
        GamepadRightTrigger =          0xCA,  // reserved  
        GamepadDpadUp =                0xCB,  // reserved  
        GamepadDpadDown =              0xCC,  // reserved  
        GamepadDpadLeft =              0xCD,  // reserved  
        GamepadDpadRight =             0xCE,  // reserved  
        GamepadMenu =                  0xCF,  // reserved  
        GamepadView =                  0xD0,  // reserved  
        GamepadLeftThumbstickButton =  0xD1,  // reserved  
        GamepadRightThumbstickButton = 0xD2,  // reserved  
        GamepadLeftThumbstickUp =      0xD3,  // reserved  
        GamepadLeftThumbstickDown =    0xD4,  // reserved  
        GamepadLeftThumbstickRight =   0xD5,  // reserved  
        GamepadLeftThumbstickLeft =    0xD6,  // reserved  
        GamepadRightThumbstickUp =     0xD7,  // reserved  
        GamepadRightThumbstickDown =   0xD8,  // reserved  
        GamepadRightThumbstickRight =  0xD9,  // reserved  
        GamepadRightThumbstickLeft =   0xDA,  // reserved  
        Oem4 =                         0xDB,  //  '[{' for US
        Oem5 =                         0xDC,  //  '\|' for US
        Oem6 =                         0xDD,  //  ']}' for US
        Oem7 =                         0xDE,  //  ''"' for US
        Oem8 =                         0xDF,
        OemAx =                        0xE1,  //  'AX' key on Japanese AX kbd
        Oem102 =                       0xE2,  //  "<>" or "\|" on RT 102-key kbd.
        IcoHelp =                      0xE3,  //  Help key on ICO
        Ico00 =                        0xE4,  //  00 key on ICO
        ProcessKey =                   0xE5,
        IcoClear =                     0xE6,
        Packet =                       0xE7,
        OemReset =                     0xE9,
        OemJump =                      0xEA,
        OemPa1 =                       0xEB,
        OemPa2 =                       0xEC,
        OemPa3 =                       0xED,
        OemWsCtrl =                    0xEE,
        OemCuSel =                     0xEF,
        OemAttn =                      0xF0,
        OemFinish =                    0xF1,
        OemCopy =                      0xF2,
        OemAuto =                      0xF3,
        OemEnlw =                      0xF4,
        OemBackTab =                   0xF5,
        Attn =                         0xF6,
        CrSel =                        0xF7,
        ExSel =                        0xF8,
        ErEof =                        0xF9,
        Play =                         0xFA,
        Zoom =                         0xFB,
        NoName =                       0xFC,
        Pa1 =                          0xFD,
        OemClear =                     0xFE
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
        void put(const Coord& position, wchar_t character, Word attributes)
        {
            mat[position.y * siz.x + position.x] = CharInfo{ character, attributes };
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
