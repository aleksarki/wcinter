# WCI - Windows console interface

## [WCI](../wci.md) > Enumeration `ButtonState`

Enumeration `ButtonState` is based off type [`Dword`](../types/dword.md) and provides following options:

- `FromLeft1stButtonPressed` = `0x01`
- `RightmostButtonPressed`   = `0x02`
- `FromLeft2ndButtonPressed` = `0x04`
- `FromLeft3rdButtonPressed` = `0x08`
- `FromLeft4thButtonPressed` = `0x10`

Following operators are overloaded for this enumeration:

- `constexpr ButtonState operator|(ButtonState a, ButtonState b);`
- `constexpr ButtonState operator&(ButtonState a, ButtonState b);`
- `constexpr ButtonState operator^(ButtonState a, ButtonState b);`
