# WCI - Windows console interface

WCI is a C++ library that allows for advanced console manipulations (apart from mere input and output).
This library is built upon Win32, utilises its API and prevents it from polluting global namespace with various definitions.

## Types

- [`Wchar`](./types/wchar.md)
- [`Short`](./types/short.md)
- [`Word`](./types/word.md)
- [`Dword`](./types/dword.md)
- [`Handle`](./types/handle.md)


## Enumerations

- [`Attribute`](./enums/attribute.md)
- [`ButtonState`](./enums/button_state.md)
- [`CodePage`](./enums/code_page.md)
- [`ControlKeyState`](./enums/control_key_state.md)
- [`EventFlag`](./enums/event_flag.md)
- [`EventType`](./enums/event_type.md)
- [`FileAccessRights`](./enums/file_access_rights.md)
- [`GenericRights`](./enums/generic_rights.md)
- [`InputMode`](./enums/input_mode.md)
- [`StdHandle`](./enums/std_handle.md)
- [`VirtualKEy`](./enums/virtual_key.md)


## Structures

- [`CharInfo`](./structs/char_info.md)
- [`Coord`](./structs/coord.md)
- [`CursorInfo`](./structs/cursor_info.md)
- [`FocusEventRecord`](./structs/focus_event_record.md)
- [`InputRecord`](./structs/input_record.md)
- [`KeyEventRecord`](./structs/key_event_record.md)
- [`MenuEventRecord`](./structs/menu_event_record.md)
- [`MouseEventRecord`](./structs/mouse_event_record.md)
- [`ScreenBufferInfo`](./structs/screen_buffer_info.md)
- [`SmallRect`](./structs/small_rect.md)
- [`WindowBufferSizeRecord`](./structs/window_buffer_size_record.md)


## Classes

- [`CharMatrix`](./classes/char_matrix.md)
- [`Console`](./classes/console.md)
