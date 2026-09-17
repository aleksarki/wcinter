# WCI - Windows console interface

## [WCI](../wci.md) > Structure `ScreenBufferInfo`

Structure `ScreenBufferInfo` contains following fields:

- [`Coord`](./coord.md) `size`
- [`Coord`](./coord.md) `cursorPosition`
- [`Attribute`](../enums//attribute.md) `attributes`
- [`SmallRect`](./small_rect.md) `window`
- [`Coord`](./coord.md) `maxWindowSize`

Following operators are overloaded for this structure:

- `constexpr bool operator==(const ScreenBufferInfo& a, const ScreenBufferInfo& b) noexcept;`
- `constexpr bool operator!=(const ScreenBufferInfo& a, const ScreenBufferInfo& b) noexcept;`

---

Structure `ScreenBufferInfo` contains information about the console screen buffer.
