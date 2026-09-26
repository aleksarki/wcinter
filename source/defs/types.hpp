#pragma once
#ifndef CINTER_INCLUDE_DEFINITIONS_TYPES_HPP
#define CINTER_INCLUDE_DEFINITIONS_TYPES_HPP

#include <variant>

namespace wci
{
    using Wchar = wchar_t;
    using Short = short;
    using Word = unsigned short;
    using Dword = unsigned long;
    using Handle = void*;
    using LenPercent = std::variant<Short, double>;  // either absolute or relative coordinate
}

#endif  // CINTER_INCLUDE_DEFINITIONS_TYPES_HPP
