# WCI - Windows console interface

## [WCI](../wci.md) > Enumeration `FileAccessRights`

Enumeration `FileAccessRights` is based off type [`Dword`](../types/dword.md) and provides following options:

- `ShareRead` =   `0x00000001`
- `ShareWrite` =  `0x00000002`
- `ShareDelete` = `0x00000004`

Following operators are overloaded for this enumeration:

- `constexpr FileAccessRights operator|(FileAccessRights a, FileAccessRights b);`
- `constexpr FileAccessRights operator&(FileAccessRights a, FileAccessRights b);`
- `constexpr FileAccessRights operator^(FileAccessRights a, FileAccessRights b);`
