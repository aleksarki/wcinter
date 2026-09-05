# WCI - Windows console interface

## [WCI](../wci.md) > Enumeration `InputMode`

Enumeration `InputMode` provides following options:

- `EnableProcessedInput` =       `0x01`
- `EnableLineInput` =            `0x02`
- `EnableEchoInput` =            `0x04`
- `EnableWindowsInput` =         `0x08`
- `EnableMouseInput` =           `0x10`
- `EnableInsertMode` =           `0x20`
- `EnableQuickEditMode` =        `0x40`
- `EnableExtendedFlags` =        `0x80`
- `EnableAutoPosition` =         `0x0100`
- `EnableVirtualTerminalInput` = `0x0200`

Following operators are overloaded for this enumeration:

- `constexpr InputMode operator|(InputMode a, InputMode b);`
- `constexpr InputMode operator&(InputMode a, InputMode b);`
- `constexpr InputMode operator^(InputMode a, InputMode b);`
