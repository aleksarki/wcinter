#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <forward_list>
#include <functional>
#include <memory>
#include "include/console.hpp"
#include "include/eventloop.hpp"

class wci::EventLoop::Impl
{
private:
    wci::Console& console;
    std::forward_list<std::function<void(wci::KeyEventRecord&)>> keyEventBindings;
    std::forward_list<std::function<void(wci::MouseEventRecord&)>> mouseEventBindings;
    std::forward_list<std::function<void(wci::WindowBufferSizeRecord&)>> windowBufferSizeEventBindings;
    
public:
    Impl(wci::Console& console) : console(console), keyEventBindings{}, mouseEventBindings{}, windowBufferSizeEventBindings{} {}

    void bindKeyEvent(std::function<void(KeyEventRecord&)> callback)
    {
        keyEventBindings.push_front(std::move(callback));
    }
    void bindMouseEvent(std::function<void(MouseEventRecord&)> callback)
    {
        mouseEventBindings.push_front(std::move(callback));
    }
    void bindWindowBufferSizeEvent(std::function<void(WindowBufferSizeRecord&)> callback)
    {
        windowBufferSizeEventBindings.push_front(std::move(callback));
    }

    void execute(bool& proceed)
    {
        wci::InputRecord inputBuffer[128];
        wci::Dword eventsRead;

        while (proceed)
        {
            console.readInput(inputBuffer, (wci::Dword)128, &eventsRead);
            for (wci::Dword i = 0; i < eventsRead; ++i)
            {
                switch (inputBuffer[i].eventType)
                {
                case wci::EventType::KeyEvent:
                    for (auto& callback : keyEventBindings)
                        callback(inputBuffer[i].event.keyEvent);
                    break;

                case wci::EventType::MouseEvent:
                    for (auto& callback : mouseEventBindings)
                        callback(inputBuffer[i].event.mouseEvent);
                    break;

                case wci::EventType::WindowBufferSizeEvent:
                    for (auto& callback : windowBufferSizeEventBindings)
                        callback(inputBuffer[i].event.windowBufferSizeEvent);
                    break;
                }
            }
        }
    }
};

wci::EventLoop::EventLoop(wci::Console& console) : pImpl(std::make_unique<Impl>(console)) {}
wci::EventLoop::~EventLoop() = default;

void wci::EventLoop::bindKeyEvent(std::function<void(KeyEventRecord&)> callback)
{
    pImpl->bindKeyEvent(std::move(callback));
}
void wci::EventLoop::bindMouseEvent(std::function<void(MouseEventRecord&)> callback)
{
    pImpl->bindMouseEvent(std::move(callback));
}
void wci::EventLoop::bindWindowBufferSizeEvent(std::function<void(WindowBufferSizeRecord&)> callback)
{
    pImpl->bindWindowBufferSizeEvent(std::move(callback));
}

void wci::EventLoop::execute(bool& proceed)
{
    pImpl->execute(proceed);
}
