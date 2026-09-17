#define UNICODE
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <stdexcept>
#include <thread>
#include <windows.h>
#include <forward_list>
#include "include/frameloop.hpp"
#include "include/apicast.hpp"

using namespace std::chrono_literals;

class wci::FrameLoop::Impl
{
private:
    wci::Console& console;
    std::forward_list<std::function<void()>> onTickBindings;
    std::forward_list<std::function<void()>> onEventBindings;
    std::forward_list<std::function<void(const wci::KeyEventRecord&)>> keyEventBindings;
    std::forward_list<std::function<void(const wci::MouseEventRecord&)>> mouseEventBindings;
    std::forward_list<std::function<void(const wci::WindowBufferSizeRecord&)>> resizeEventBindings;
    std::chrono::milliseconds frameTime;
    bool proceed;  // idea make atomic

public:
    Impl(wci::Console& console) :
        console(console), frameTime(16ms), onTickBindings(), onEventBindings(),
        keyEventBindings(), mouseEventBindings(), resizeEventBindings()
    {}

    void frameTime(std::chrono::milliseconds ft)
    {
        if (ft <= 0ms)
            throw std::invalid_argument("FrameLoop::frameTime() got invalid frame time");
        frameTime = ft;
    }
    std::chrono::milliseconds frameTime() const noexcept
    {
        return frameTime;
    }

    void framesPerSecond(unsigned fps)
    {
        if (fps == 0)
            throw std::invalid_argument("FrameLoop::framesPerSecond() got invalid amount");
        frameTime = std::max(1000ms / fps, 1ms);
    }
    unsigned framesPerSecond() const noexcept
    {
        return 1000ms / frameTime;
    }

    void bindOnTick(std::function<void()> callback)
    {
        onTickBindings.push_front(std::move(callback));
    }

    void bindOnEvents(std::function<void()> callback)
    {
        onEventBindings.push_front(std::move(callback));
    }

    void bindOnKeyEvent(std::function<void(const KeyEventRecord&)> callback)
    {
        keyEventBindings.push_front(std::move(callback));
    }

    void bindOnMouseEvent(std::function<void(const MouseEventRecord&)> callback)
    {
        mouseEventBindings.push_front(std::move(callback));
    }

    void bindOnResizeEvent(std::function<void(const WindowBufferSizeRecord&)> callback)
    {
        resizeEventBindings.push_front(std::move(callback));
    }

    void run()
    {
        proceed = true;
        auto nextTime = std::chrono::steady_clock::now();

        wci::InputRecord inputBuffer[128];
        wci::Dword eventsNum = 0, eventsRead = 0;
        bool handled;

        while (proceed)
        {
            auto success = GetNumberOfConsoleInputEvents(console.stdInput(), wci::api(&eventsNum));
            if (success && eventsNum)
            {
                console.readInput(inputBuffer, (wci::Dword)128, &eventsRead);  // todo implement literals
                handled = false;
                for (wci::Dword i = 0; i < eventsRead; ++i)
                {
                    switch (inputBuffer[i].eventType)
                    {
                    case wci::EventType::KeyEvent:
                        for (const auto& callback : keyEventBindings)
                            callback(inputBuffer[i].event.keyEvent);
                        handled = true;
                        break;

                    case wci::EventType::MouseEvent:
                        for (const auto& callback : mouseEventBindings)
                            callback(inputBuffer[i].event.mouseEvent);
                        handled = true;
                        break;

                    case wci::EventType::WindowBufferSizeEvent:
                        for (const auto& callback : resizeEventBindings)
                            callback(inputBuffer[i].event.windowBufferSizeEvent);
                        handled = true;
                        break;
                    }
                }

                if (handled)
                    for (const auto& callback : onEventBindings)
                        callback();
            }

            for (const auto& callback : onTickBindings)
                callback();

            nextTime += frameTime;
            std::this_thread::sleep_until(nextTime);
        }
    }

    void stop() noexcept
    {
        proceed = false;
    }
};

wci::FrameLoop::FrameLoop(wci::Console& console) : impl(std::make_unique<wci::FrameLoop::Impl>(console)) {}
wci::FrameLoop::~FrameLoop() = default;

void wci::FrameLoop::frameTime(std::chrono::milliseconds ft)
{
    impl->frameTime(ft);
}
std::chrono::milliseconds wci::FrameLoop::frameTime() const noexcept
{
    return impl->frameTime();
}

void wci::FrameLoop::framesPerSecond(unsigned fps)
{
    impl->framesPerSecond(fps);
}
unsigned wci::FrameLoop::framesPerSecond() const noexcept
{
    return impl->framesPerSecond();
}

void wci::FrameLoop::bindOnTick(std::function<void()> callback)
{
    impl->bindOnTick(std::move(callback));
}

void wci::FrameLoop::bindOnEvents(std::function<void()> callback)
{
    impl->bindOnEvents(std::move(callback));
}

void wci::FrameLoop::bindOnKeyEvent(std::function<void(const wci::KeyEventRecord &)> callback)
{
    impl->bindOnKeyEvent(std::move(callback));
}

void wci::FrameLoop::bindOnMouseEvent(std::function<void(const wci::MouseEventRecord&)> callback)
{
    impl->bindOnMouseEvent(std::move(callback));
}

void wci::FrameLoop::bindOnResizeEvent(std::function<void(const wci::WindowBufferSizeRecord&)> callback)
{
    impl->bindOnResizeEvent(std::move(callback));
}

void wci::FrameLoop::run()
{
    impl->run();
}

void wci::FrameLoop::stop() noexcept
{
    impl->stop();
}
