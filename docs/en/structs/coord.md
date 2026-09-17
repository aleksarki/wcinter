# WCI - Windows console interface

## [WCI](../wci.md) > Structure `Coord`

Structure `Coord` contains following fields:

- [`Short`](../types/short.md) `x`
- [`Short`](../types/short.md) `y`

Following operators are overloaded for this structure:

- `constexpr bool operator==(const Coord& a, const Coord& b) noexcept;`
- `constexpr bool operator!=(const Coord& a, const Coord& b) noexcept;`

---

Structure `Coord` defines the coordinates of a character cell in  the console screen buffer.
The origin of the coordinate system (0; 0) is the upper-left cell of the buffer.
