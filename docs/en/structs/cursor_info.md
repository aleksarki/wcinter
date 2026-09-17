# WCI - Windows console interface

## [WCI](../wci.md) > Structure `SmallRect`

Structure `SmallRect` contains following fields:

- [`Dword`](../types/dword.md) `size`;
- `bool` `visible`;

Following operators are overloaded for this structure:

- `constexpr bool operator==(const CursorInfo& a, const CursorInfo& b) noexcept;`
- `constexpr bool operator!=(const CursorInfo& a, const CursorInfo& b) noexcept;`

---

Structure `CursorInfo` contains information about the console cursor.
