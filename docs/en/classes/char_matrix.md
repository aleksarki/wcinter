# WCI - Windows console interface

## [WCI](../wci.md) > Class `CharMatrix`

Class `CharMatrix` contains following public fields:

- `static constexpr CharInfo` `nullChar` = `{ 0, Attribute::No }`

Class `CharMatrix` contains following public methods:

```cpp
CharMatrix(Short x, Short y) noexcept;
CharMatrix(const Coord& size) noexcept;
```

Construct an instance of the class. New matrix will be automatically filled with black null characters (`nullChar`).

---
```cpp
CharInfo& at(Short x, Short y);
CharInfo& at(const Coord& position);
const CharInfo& at(Short x, Short y) const;
const CharInfo& at(const Coord& position) const;
```

Get `CharInfo` object by its position inside matrix. Trows `std::out_of_bounds` when out of bounds.

---
```cpp
CharInfo& operator[](const Coord& position);
const CharInfo& operator[](const Coord& position) const;
```

Get a `CharInfo` object by its position inside matrix. Trows `std::out_of_bounds` when out of bounds.

---
```cpp
void put(Short x, Short y, wchar_t character, Attribute attributes);
void put(Short x, Short y, const CharInfo& charInfo);
void put(const Coord& position, wchar_t character, Attribute attributes);
void put(const Coord& position, const CharInfo& charInfo);
```

Place a `CharInfo` object inside the matrix in certain position. Trows `std::out_of_bounds` when out of bounds.

---
```cpp
CharInfo* data() noexcept;
const CharInfo* data() const noexcept;
```

Get access to the first element of the inner vector holding the data.

---
```cpp
const Coord size() const noexcept;
```

Get current dimensions of the matrix.

---
```cpp
void resize(Short x, Short y);
void resize(const Coord& size);
void resize(Short x, Short y, wchar_t character, Attribute attributes);
void resize(const Coord& size, const CharInfo& charInfo);
```

Resize the matrix to new dimensions and fill it with a `CharInfo` object.

---
```cpp
void fill(wchar_t character, Attribute attributes);
void fill(const CharInfo& charInfo);
```

Fill the matrix with a `CharInfo` object.

---
```cpp
void blank();
```

Fill the with black null characters (`nullChar`).

---
```cpp
bool within(Short x, Short y) const noexcept;
bool within(const Coord& position) const noexcept;
```

Check whether a given positions is within the matrix.

---

Class `CharMatrix` represents rectangular field of cells containing characters and their attributes.
