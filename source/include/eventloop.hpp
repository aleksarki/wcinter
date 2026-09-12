#pragma once
#ifndef CINTER_INCLUDE_EventLoop_HPP
#define CINTER_INCLUDE_EventLoop_HPP

#include <functional>
#include <memory>
#include "definitions.hpp"
#include "console.hpp"

namespace wci {

    class EventLoop
    {
    public:
        EventLoop(Console& console);
        ~EventLoop();

        void bindKeyEvent(std::function<void(const KeyEventRecord&)> callback);

        void bindMouseEvent(std::function<void(const MouseEventRecord&)> callback);

        void bindWindowBufferSizeEvent(std::function<void(const WindowBufferSizeRecord&)> callback);

        void execute(bool& proceed);

    private:
        class Impl;
        std::unique_ptr<Impl> impl;
    };

}

#endif  // CINTER_INCLUDE_EventLoop_HPP
