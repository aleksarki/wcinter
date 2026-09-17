#pragma once
#ifndef CINTER_INCLUDE_FRAMELOOP_HPP
#define CINTER_INCLUDE_FRAMELOOP_HPP

#include <chrono>
#include <functional>
#include <memory>
#include "console.hpp"
#include "definitions.hpp"

namespace wci
{
    class FrameLoop
    {
    public:
        FrameLoop(Console& console);
        ~FrameLoop();

        void frameTime(std::chrono::milliseconds ft);
        std::chrono::milliseconds frameTime() const noexcept;

        void framesPerSecond(unsigned fps);
        unsigned framesPerSecond() const noexcept;

        void bindOnTick(std::function<void()> callback);

        void bindOnEvents(std::function<void()> callback);

        void bindOnKeyEvent(std::function<void(const KeyEventRecord&)> callback);

        void bindOnMouseEvent(std::function<void(const MouseEventRecord&)> callback);

        void bindOnResizeEvent(std::function<void(const WindowBufferSizeRecord&)> callback);

        void run();

        void stop() noexcept;

    private:
        class Impl;
        std::unique_ptr<Impl> impl;
    };
}

#endif  // CINTER_INCLUDE_FRAMELOOP_HPP
