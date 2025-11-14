#pragma once
#ifndef CINTER_INCLUDE_CONSOLE_HPP
#define CINTER_INCLUDE_CONSOLE_HPP

#include <memory>

namespace cinter {

    class Console
    {
    private:
        class Impl;
        std::unique_ptr<Impl> pImpl;

    public:
        Console();
        ~Console();

        Console(const Console&) = delete;
        Console& operator=(const Console&) = delete;

        Console(Console&&) = default;
        Console& operator=(Console&&) = default;
    };

}

#endif  // CINTER_INCLUDE_CONSOLE_HPP
