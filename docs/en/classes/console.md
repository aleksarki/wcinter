# WCI - Windows console interface

## [WCI](../wci.md) > Class `Console`

Class `Console` contains following public methods:

```cpp
Console();
```

Construct an instance of the class. Console will be set in new mode, enabling winows and mouse input; and code page will be set to UTF-8. Old console mode and code page will be restored upon deconstruction.

---
```cpp
Console(const Console&) = delete;
Console& operator=(const Console&) = delete;
```

Copying is prohibited.

---
```cpp
void write(const char* string);
void write(const std::string& string);
void write(const wchar_t* wstring);
void write(const std::wstring& wstring);
```

Write a string to the console in current cursor position and move it after.

---
```cpp
std::wstring title() const;
```

Get current console title.

---
```cpp
void title(const std::wstring& newTitle);
```

Set new console title.

---
```cpp
ScreenBufferInfo screenBufferInfo() const;
```

Get description of console screen buffer.

---
```cpp
CursorInfo cursorInfo() const;
```

Get description of console cursor.

---
```cpp
void cursorInfo(const CursorInfo& info);
```

Set new properties of console cursor.

---
```cpp
void cursorPosition(const Coord& position);
```

Set new console cursor position.

---
```cpp
Handle activeScreenBuffer() const;
```

Get handle of current active screen buffer.

---
```cpp
void activeScreenBuffer(Handle handle);
```

Set new active screen buffer.

---
```cpp
void textAttribute(Word attributes);
```

Set attribute for text printed to console.

---
```cpp
void readInput(InputRecord* inputBuffer, Dword inputBufferLength, Dword* eventsRead);
```

Read input events of console.

---
```cpp
Handle stdInput() const;
```

Get standard input handle of the console.

---
```cpp
Handle stdOutput() const;
```

Get standard output handle of the console.

---
```cpp
Handle stdError() const;
```

Get standard error handle of the console.

---

Class `Console` represents regular console.
