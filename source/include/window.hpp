#pragma once
#ifndef CINTER_INCLUDE_WINDOW_HPP
#define CINTER_INCLUDE_WINDOW_HPP

#include <memory>
#include "console.hpp"

namespace cinter {

    class Window
    {
    private:
        class Impl;
        std::unique_ptr<Impl> pImpl;
    
    public:
        Window();
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        Window(Window&&) = default;
        Window& operator=(Window&&) = default;
    };
}

#endif  // CINTER_INCLUDE_WINDOW_HPP
