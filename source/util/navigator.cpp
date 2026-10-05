#include <stdexcept>
#include <utility>

#include "navigator.hpp"
#include "page.hpp"

wci::Navigator::Navigator() : pages(), requests()
{}

wci::Navigator::~Navigator()
{
    while (!empty())
    {
        peek().destroy();
        pages.pop();
    }
}

bool wci::Navigator::empty() const noexcept
{
    return pages.empty();
}

wci::Page& wci::Navigator::peek()
{
    if (empty())
        throw std::logic_error("Navigator is empty");
    return *pages.top();
}

void wci::Navigator::push(std::unique_ptr<wci::Page> page)
{
    if (!empty())
        peek().hide();
    pages.push(std::move(page));
    peek().build();
    peek().show();
}

void wci::Navigator::pop()
{
    if (empty())
        throw std::logic_error("Navigator is empty");
    peek().hide();
    peek().destroy();
    pages.pop();
    if (!empty())
        peek().show();
}

void wci::Navigator::replace(std::unique_ptr<wci::Page> page)
{
    if (empty())
    {
        push(std::move(page));
        return;
    }
    peek().hide();
    peek().destroy();
    pages.pop();
    pages.push(std::move(page));
    peek().build();
    peek().show();
}

void wci::Navigator::requestPush(std::unique_ptr<wci::Page> page)
{
    requests.push(wci::Navigator::Request{ wci::Navigator::Action::Push, std::move(page) });
}

void wci::Navigator::requestPop()
{
    requests.push(wci::Navigator::Request{ wci::Navigator::Action::Pop });
}

void wci::Navigator::requestReplace(std::unique_ptr<wci::Page> page)
{
    requests.push(wci::Navigator::Request{ wci::Navigator::Action::Replace, std::move(page) });
}

void wci::Navigator::satisfy()
{
    while (!requests.empty())
    {
        auto& request = requests.front();
        switch (request.action)
        {
        case wci::Navigator::Action::Push:
            push(std::move(request.page));
            break;
        case wci::Navigator::Action::Pop:
            pop();
            break;
        case wci::Navigator::Action::Replace:
            replace(std::move(request.page));
            break;
        }
        requests.pop();
    }
}

bool wci::Navigator::dispatchTick()
{
    bool handled = false;
    if (!empty())
    {
        handled = peek().onTick();
        satisfy();
    }
    return handled;
}

bool wci::Navigator::dispatchKeyEvent(const wci::KeyEvent& event)
{
    bool handled = false;
    if (!empty())
    {
        handled = peek().onKeyEvent(event);
        satisfy();
    }
    return handled;
}

bool wci::Navigator::dispatchMouseEvent(const wci::MouseEvent& event)
{
    bool handled = false;
    if (!empty())
    {
        handled = peek().onMouseEvent(event);
        satisfy();
    }
    return handled;
}

bool wci::Navigator::dispatchResizeEvent(const wci::WindowBufferResizeEvent& event)
{
    bool handled = false;
    if (!empty())
    {
        handled = peek().onResizeEvent(event);
        satisfy();
    }
    return handled;
}
