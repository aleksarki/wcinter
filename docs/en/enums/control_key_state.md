# WCI - Windows console interface

## [WCI](../wci.md) > Enumeration `ControlKeyState`

Enumeration `ControlKeyState` is based off type [`Dword`](../types/dword.md) and provides following options:

- `RightAltPressed` =  `0x01`:       The right alt key is pressed.
- `LeftAltPressed` =   `0x02`:       The left alt key is pressed.
- `RightCtrlPressed` = `0x04`:       The right ctrl key is pressed.
- `LeftCtrlPressed` =  `0x08`:       The left ctrl key is pressed.
- `ShiftPressed` =     `0x10`:       The shift key is pressed.
- `NumLockOn` =        `0x20`:       The numlock light is on.
- `ScrollLockOn` =     `0x40`:       The scrolllock light is on.
- `CapsLockOn` =       `0x80`:       The capslock light is on.
- `EnhancedKey` =      `0x0100`:     The key is enhanced.
- `NlsDbcsChar` =      `0x010000`:   DBCS for JPN: SBCS/DBCS mode.
- `NlsAlphaNumeric` =  `0x00`:       DBCS for JPN: Alphanumeric mode.
- `NlsKatakana` =      `0x020000`:   DBCS for JPN: Katakana mode.
- `NlsHiragana` =      `0x040000`:   DBCS for JPN: Hiragana mode.
- `NlsRoman` =         `0x400000`:   DBCS for JPN: Roman/Noroman mode.
- `NlsImeConversion` = `0x800000`:   DBCS for JPN: IME conversion.
- `AltNumpadBit` =     `0x04000000`: AltNumpad OEM char.
- `NlsImeDisable` =    `0x20000000`: DBCS for JPN: IME enable/disable.

Following operators are overloaded for this enumeration:

- `constexpr ControlKeyState operator|(ControlKeyState a, ControlKeyState b);`
- `constexpr ControlKeyState operator&(ControlKeyState a, ControlKeyState b);`
- `constexpr ControlKeyState operator^(ControlKeyState a, ControlKeyState b);`
