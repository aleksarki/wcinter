#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <forward_list>
#include <functional>
#include <memory>
#include "include/console.hpp"
#include "include/application.hpp"

namespace ci = cinter;

class ci::Application::Impl
{
private:
    ci::Console& console;
    std::forward_list<std::function<void(ci::KeyEventRecord)>> keyEventBindings;
    std::forward_list<std::function<void(ci::MouseEventRecord)>> mouseEventBindings;
    std::forward_list<std::function<void(ci::WindowBufferSizeRecord)>> windowBufferSizeEventBindings;
    
public:
    Impl(ci::Console& console) : console(console), keyEventBindings{}, mouseEventBindings{}, windowBufferSizeEventBindings{} {}

    void bindKeyEvent(std::function<void(KeyEventRecord)> callback)
    {
        keyEventBindings.push_front(std::move(callback));
    }
    void bindMouseEvent(std::function<void(MouseEventRecord)> callback)
    {
        mouseEventBindings.push_front(std::move(callback));
    }
    void bindWindowBufferSizeEvent(std::function<void(WindowBufferSizeRecord)> callback)
    {
        windowBufferSizeEventBindings.push_front(std::move(callback));
    }

    void execute()
    {
        ci::InputRecord inputBuffer[128];
        ci::Dword eventsRead;

        while (true)
        {
            console.readInput(inputBuffer, (ci::Dword)128, &eventsRead);
            for (ci::Dword i = 0; i < eventsRead; ++i)
            {
                switch (inputBuffer[i].eventType)
                {
                case static_cast<ci::Word>(ci::EventType::KeyEvent):
                    for (auto callback : keyEventBindings)
                        callback(inputBuffer[i].event.keyEvent);
                    break;

                case static_cast<ci::Word>(ci::EventType::MouseEvent):
                    for (auto callback : mouseEventBindings)
                        callback(inputBuffer[i].event.mouseEvent);
                    break;

                case static_cast<ci::Word>(ci::EventType::WindowBufferSizeEvent):
                    for (auto callback : windowBufferSizeEventBindings)
                        callback(inputBuffer[i].event.windowBufferSizeEvent);
                    break;
                }
            }
        }
    }
};

ci::Application::Application(ci::Console& console) : pImpl(std::make_unique<Impl>(console)) {}
ci::Application::~Application() = default;

void ci::Application::bindKeyEvent(std::function<void(KeyEventRecord)> callback)
{
    pImpl->bindKeyEvent(std::move(callback));
}
void ci::Application::bindMouseEvent(std::function<void(MouseEventRecord)> callback)
{
    pImpl->bindMouseEvent(std::move(callback));
}
void ci::Application::bindWindowBufferSizeEvent(std::function<void(WindowBufferSizeRecord)> callback)
{
    pImpl->bindWindowBufferSizeEvent(std::move(callback));
}

void ci::Application::execute()
{
    pImpl->execute();
}
