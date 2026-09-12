# WCI - Windows console interface

## [WCI](../wci.md) > Class `EventLoop`

Class `EventLoop` contains following public methods:

```cpp
EventLoop(Console& console);
```

Construct an instance of the class.

---
```cpp
void bindKeyEvent(std::function<void(const KeyEventRecord&)> callback);
```

Bind a callback handler to a key event.

---
```cpp
void bindMouseEvent(std::function<void(const MouseEventRecord&)> callback);
```

Bind a callback handler to a mouse event.

---
```cpp
void bindWindowBufferSizeEvent(std::function<void(const WindowBufferSizeRecord&)> callback);
```

Bind a callback handler to a buffer size event (window resize).

---
```cpp
void execute(bool& proceed);
```

Run execution loop while `proceed` is true. This reads for console input events and calls corresponding callback functions in a loop.

---

Class `EventLoop` serves for automatic handling of console input events in a loop.
