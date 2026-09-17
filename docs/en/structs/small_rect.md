# WCI - Windows console interface

## [WCI](../wci.md) > Structure `SmallRect`

Structure `SmallRect` contains following fields:

- [`Short`](../types/short.md) `left`:   X of top left corner.
- [`Short`](../types/short.md) `top`:    Y of top left corner.
- [`Short`](../types/short.md) `right`:  X of bottom right corner.
- [`Short`](../types/short.md) `bottom`: Y of bottom right corner.

Following operators are overloaded for this structure:

- `constexpr bool operator==(const SmallRect& a, const SmallRect& b) noexcept;`
- `constexpr bool operator!=(const SmallRect& a, const SmallRect& b) noexcept;`

---

Structure `SmallRect` defines coordinates of upper-left and lower-right corners of a rectangle.
