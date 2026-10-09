#pragma once
#ifndef CINTER_INCLUDE_DEFINITIONS_TYPES_HPP
#define CINTER_INCLUDE_DEFINITIONS_TYPES_HPP

#include <cstddef>

namespace wci
{
    using Wchar = wchar_t;
    using Short = short;
    using Word = unsigned short;
    using Dword = unsigned long;
    using Handle = void*;
    using Id = std::size_t;
}

#endif  // CINTER_INCLUDE_DEFINITIONS_TYPES_HPP
