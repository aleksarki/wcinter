# WCI - Windows console interface

## [WCI](../wci.md) > Class `CharMatrix`

Class `CharMatrix` has following public fields:

```cpp
static constexpr CharInfo nullChar = { 0, Attribute::No };
```

Class `CharMatrix` has following public methods:

```cpp
CharMatrix()
CharMatrix(Short x, Short y)
CharMatrix(Short x, Short y, Wchar character, Attribute attributes)
CharMatrix(const Coord& size)
CharMatrix(const Coord& size, const CharInfo& character)
```

- Construct an instance of the class. Character to fill the matrix with defaults to black null characters (`nullChar`). If size is not provided, it will be assumed zero.

```cpp
CharInfo& at(Short x, Short y);
CharInfo& at(const Coord& position);
const CharInfo& at(Short x, Short y) const;
const CharInfo& at(const Coord& position) const;
```

- Get `CharInfo` object by its position inside matrix. Trows `std::out_of_bounds` when out of bounds.

```cpp
CharInfo& operator[](const Coord& position) noexcept;
const CharInfo& operator[](const Coord& position) const noexcept;
```

- Get a `CharInfo` object by its position inside matrix. This does not check whether requested character is within bounds of matrix.

```cpp
void put(Short x, Short y, Wchar character, Attribute attributes);
void put(Short x, Short y, const CharInfo& charInfo);
void put(const Coord& position, Wchar character, Attribute attributes);
void put(const Coord& position, const CharInfo& charInfo);
```

- Place a `CharInfo` object inside the matrix at certain position. Trows `std::out_of_bounds` when out of bounds.

```cpp
CharInfo* data() noexcept;
const CharInfo* data() const noexcept;
```

- Get pointer to the first element of the inner vector holding the data.

```cpp
constexpr bool empty() const noexcept;
```

- Check whether matrix holds no elements (at least one of its dimensions is 0).

```cpp
Coord size() const noexcept;
```

- Get current dimensions of the matrix.

```cpp
void resize(Short x, Short y, Wchar character, Attribute attributes);
void resize(const Coord& size, const CharInfo& charInfo);
void resize(Short x, Short y);
void resize(const Coord& size);
```

- Resize the matrix to new dimensions and fill it with a `CharInfo` object. It fills with `nullChar` if no object is passed.

```cpp
void fill(Wchar character, Attribute attributes) noexcept;
void fill(const CharInfo& charInfo) noexcept;
```

- Fill the matrix with a `CharInfo` object.

```cpp
void blank() noexcept;
```

- Fill the matrix with black null characters (`nullChar`).

```cpp
constexpr bool within(Short x, Short y) const noexcept;
constexpr bool within(const Coord& position) const noexcept;
```

- Check whether a given positions is within bounds of the matrix.

```cpp
void swap(CharMatrix& other) noexcept;
friend void swap(CharMatrix& a, CharMatrix& b) noexcept;
```

- Swap two matrices.

```cpp
CharMatrix overlay(Short x, Short y, const CharMatrix& charMatrix) const;
CharMatrix overlay(const Coord& offset, const CharMatrix& charMatrix) const;
CharMatrix overlay(const CharMatrix& charMatrix) const;
```

- Produce new matrix which is a copy of current one with the other one stacked upon it at a certain offset. Elements of the other matrix that do not fit are ignored.

```cpp
void inlay(Short x, Short y, const CharMatrix& charMatrix);
void inlay(const Coord& offset, const CharMatrix& charMatrix);
void inlay(const CharMatrix& charMatrix);
```

- Place elements of other matrix on the current one at a certain offset. Elements of the other matrix that do not fit are ignored.

```cpp
CharMatrix slice(Short x1, Short y1, Short x2, Short y2) const;
CharMatrix slice(const Coord& topLeft, const Coord& bottomRight) const;
```

- Produce new matrix that is a slice of the current one. The bottom-right point is excluded from resultant matrix.

---

Class `CharMatrix` represents rectangular field of cells containing characters and their attributes.
