#pragma once
#ifndef CINTER_INCLUDE_WINDOW_HPP
#define CINTER_INCLUDE_WINDOW_HPP

#include <memory>
#include "definitions.hpp"
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

        Console& console() noexcept;
        const Console& console() const noexcept;

        CharMatrix& matrix() noexcept;
        const CharMatrix& matrix() const noexcept;

        void render();
    };
}

#endif  // CINTER_INCLUDE_WINDOW_HPP
