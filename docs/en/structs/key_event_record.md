# WCI - Windows console interface

## [WCI](../wci.md) > Structure `KeyEventRecord`

Structure `KeyEventRecord` contains following fields:

- `bool` `keyDown`
- [`Word`](../types/word.md) `repeatCount`
- [`VirtualKey`](../enums/virtual_key.md) `virtualKeyCode`
- [`VirtualKey`](../enums/virtual_key.md) `virtualScanCode`
- [`Wchar`](../types/wchar.md) `character`
- [`ControlKeyState`](../enums/control_key_state.md) `controlKeyState`

---

Structure `KeyEventRecord` is used for recording keyboard input events for structure [`InputRecord`](./input_record.md).
