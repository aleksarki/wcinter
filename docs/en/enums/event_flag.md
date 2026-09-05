# WCI - Windows console interface

## [WCI](../wci.md) > Enumeration `EventFlag`

Enumeration `EventFlag` is based off type [`Dword`](../types/dword.md) and provides following options:

- `MouseMoved` =    `0x01`
- `DoubleClick` =   `0x02`
- `MouseWheeled` =  `0x04`
- `MouseHWheeled` = `0x08`

Following operators are overloaded for this enumeration:

- `constexpr EventFlag operator|(EventFlag a, EventFlag b);`
- `constexpr EventFlag operator&(EventFlag a, EventFlag b);`
- `constexpr EventFlag operator^(EventFlag a, EventFlag b);`
