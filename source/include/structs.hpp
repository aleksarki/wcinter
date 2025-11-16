#pragma once
#ifndef CINTER_INCLUDE_STRUCTS_HPP
#define CINTER_INCLUDE_STRUCTS_HPP

namespace cinter {

    using Short = short;
    using Word = unsigned short;
    using Dword = unsigned long;

    struct Coord
    {
        Short x;
        Short y;
    };

    struct SmallRect
    {
        Short left;    // X of top left corner.
        Short top;     // Y of top left corner.
        Short right;   // X of bottom right corner.
        Short bottom;  // Y of bottom right corner.
    };

    struct CharInfo
    {
        wchar_t character;
        Word attributes;
    };

    struct CursorInfo
    {
        Dword size;
        bool visible;
    };

    struct ScreenBufferInfo
    {
        Coord size;
        Coord cursorPosition;
        Word attributes;
        SmallRect window;
        Coord maxWindowSize;
    };

}

#endif  // CINTER_INCLUDE_STRUCTS_HPP
