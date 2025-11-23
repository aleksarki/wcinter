#pragma once
#ifndef CINTER_INCLUDE_APPLICATION_HPP
#define CINTER_INCLUDE_APPLICATION_HPP

#include <functional>
#include <memory>
#include "definitions.hpp"
#include "console.hpp"

namespace cinter {

    class Application
    {
    public:
        Application(Console& console);
        ~Application();

        void bindKeyEvent(std::function<void(KeyEventRecord)> callback);
        void bindMouseEvent(std::function<void(MouseEventRecord)> callback);
        void bindWindowBufferSizeEvent(std::function<void(WindowBufferSizeRecord)> callback);

        void execute();
    
    private:
        class Impl;
        std::unique_ptr<Impl> pImpl;
    };

}

#endif  // CINTER_INCLUDE_APPLICATION_HPP
