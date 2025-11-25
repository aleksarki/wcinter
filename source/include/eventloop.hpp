#pragma once
#ifndef CINTER_INCLUDE_EventLoop_HPP
#define CINTER_INCLUDE_EventLoop_HPP

#include <functional>
#include <memory>
#include "definitions.hpp"
#include "console.hpp"

namespace cinter {

    class EventLoop
    {
    public:
        EventLoop(Console& console);
        ~EventLoop();

        void bindKeyEvent(std::function<void(KeyEventRecord)> callback);
        void bindMouseEvent(std::function<void(MouseEventRecord)> callback);
        void bindWindowBufferSizeEvent(std::function<void(WindowBufferSizeRecord)> callback);

        void execute(bool& proceed);
    
    private:
        class Impl;
        std::unique_ptr<Impl> pImpl;
    };

}

#endif  // CINTER_INCLUDE_EventLoop_HPP
