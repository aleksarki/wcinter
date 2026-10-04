#pragma once
#ifndef WCI_SOURCE_CORE_EVENTLOOP_HPP
#define WCI_SOURCE_CORE_EVENTLOOP_HPP

#include <functional>
#include <memory>

#include "../defs.hpp"
#include "console.hpp"

namespace wci {

    class EventLoop
    {
    public:
        EventLoop(Console& console);
        ~EventLoop();

        void bindKeyEvent(std::function<void(const KeyEvent&)> callback);

        void bindMouseEvent(std::function<void(const MouseEvent&)> callback);

        void bindWindowBufferSizeEvent(std::function<void(const WindowBufferResizeEvent&)> callback);

        void execute(bool& proceed);

    private:
        class Impl;
        std::unique_ptr<Impl> impl;
    };

}

#endif  // WCI_SOURCE_CORE_EVENTLOOP_HPP
