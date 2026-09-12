# WCI - Windows console interface

## [WCI](../wci.md) > Class `Window`

Class `Window` contains following public methods:

```cpp
Console();
```

Construct an instance of the class. It creates new console screen buffer, the old one will be restored upon deconstruction; thus providing new blank screen for use.

---
```cpp
Window(const Window&) = delete;
Window& operator=(const Window&) = delete;
```

Copying is prohibited.

---
```cpp
Console& console() noexcept;
const Console& console() const noexcept;
```

Get underlying `Console` instance.

---
```cpp
CharMatrix& matrix() noexcept;
const CharMatrix& matrix() const noexcept;
```

Get underlying `CharMatrix` instance.

---
```cpp
const Coord size() const;
```

Get current dimensions of the window. If window has been resized, `Window::resize()` should be called beforehand.

---
```cpp
void resize();
```

Actualize dimensions of the window. Underlying matrix will be expanded or clamped accordingly. This method should be called after the window has been resized.

---
```cpp
void render();
```

Render the matrix content on the Windows console, thus updating what is displayed on the screen. This method calls `Window::resize()`.

---
```cpp
void printChar(Wchar character);
```

Print a character at current cursor position, then shift it. If window has been resized, `Window::resize()` should be called beforehand.

---
```cpp
void printString(const Wchar* string);
void printString(const std::wstring& string);
```

Print a string at the current cursor position, then shift it. If window has been resized, `Window::resize()` should be called beforehand.

---
```cpp
void putChar(Short x, Short y, Wchar character);
void putChar(Short x, Short y, Wchar character, Attribute attributes);
void putChar(const Coord& position, CharInfo charInfo);
```

Put a character at a given position. If window has been resized, `Window::resize()` should be called beforehand.

---
```cpp
void putString(Short x, Short y, const Wchar* string);
void putString(Short x, Short y, const Wchar* string, Attribute attributes);
void putString(const Coord& position, const CharInfo charInfos[], size_t length);
```

Put a character string at a given position. If window has been resized, `Window::resize()` should be called beforehand.

---

Class `Window` represents blank window without scrolling. This class is suited for drawing on it and rendering, to the contrast of mere printing to it.
