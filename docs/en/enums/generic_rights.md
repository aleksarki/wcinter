# WCI - Windows console interface

## [WCI](../wci.md) > Enumeration `GenericRights`

Enumeration `GenericRights` is based off type [`Dword`](../types/dword.md) and provides following options:

- `Read` =    `0x80000000L`
- `Write` =   `0x40000000L`
- `Execute` = `0x20000000L`
- `All` =     `0x10000000L`

Following operators are overloaded for this enumeration:

- `constexpr GenericRights operator|(GenericRights a, GenericRights b);`
- `constexpr GenericRights operator&(GenericRights a, GenericRights b);`
- `constexpr GenericRights operator^(GenericRights a, GenericRights b);`