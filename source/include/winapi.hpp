#pragma once
#ifndef CINTER_INCLUDE_WINAPI_HPP
#define CINTER_INCLUDE_WINAPI_HPP

#include <windows.h>
#include "./definitions.hpp"

namespace wci {

    consteval DWORD api(StdHandle constant)
    {
        return static_cast<DWORD>(constant);
    }

    consteval int api(InputMode constant)
    {
        return static_cast<int>(constant);
    }

}

#endif  // CINTER_INCLUDE_WINAPI_HPP