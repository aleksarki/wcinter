# WCI - Windows console interface

## [WCI](../wci.md) > Structure `FocusEventRecord`

Structure `FocusEventRecord` contains following fields:

- [`EventType`](../enums/event_type.md) `eventType`
- `union` `event` of:
    - [`KeyEventRecord`](./key_event_record.md) `keyEvent`
    - [`MouseEventRecord`](./mouse_event_record.md) `mouseEvent`
    - [`WindowBufferSizeRecord`](./window_buffer_size_record.md) `windowBufferSizeEvent`
    - [`MenuEventRecord`](./menu_event_record.md) `menuEvent`
    - [`FocusEventRecord`](./focus_event_record.md) `focusEvent`

---

Structure `InputRecord` is used for recording data input events of console input buffer.
