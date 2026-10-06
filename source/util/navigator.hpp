#pragma once
#ifndef WCI_SOURCE_UTIL_NAVIGATOR_HPP
#define WCI_SOURCE_UTIL_NAVIGATOR_HPP

#include <memory>
#include <queue>
#include <stack>
#include <stdexcept>
#include <utility>

#include "../defs.hpp"
#include "page.hpp"

namespace wci
{
    class Navigator
    {
    private:
        std::stack<std::unique_ptr<Page>> pages;
        enum class Action
        {
            Push,
            Pop,
            Replace
        };
        struct Request
        {
            Action action;
            std::unique_ptr<Page> page = nullptr;
        };
        std::queue<Request> requests;

        void satisfy();

    public:
        Navigator();

        ~Navigator();

        bool empty() const noexcept;

        Page& peek();

        void push(std::unique_ptr<Page> page);

        template<page P, typename... Args>
        void emplace(Args&&... args)
        {
            push(std::make_unique<P>(std::forward<Args>(args)...));
        }

        void pop();

        void replace(std::unique_ptr<Page> page);

        void requestPush(std::unique_ptr<Page> page);

        template<page P, typename... Args>
        void requestEmplace(Args&&... args)
        {
            requestPush(std::make_unique<P>(std::forward<Args>(args)...));
        }

        void requestPop();

        void requestReplace(std::unique_ptr<Page> page);

        bool dispatchTick();

        bool dispatchKeyEvent(const KeyEvent& event);

        bool dispatchMouseEvent(const MouseEvent& event);

        bool dispatchResizeEvent(const WindowBufferResizeEvent& event);
    };
}

inline wci::Navigator::Navigator() : pages(), requests()
{}

inline wci::Navigator::~Navigator()
{
    while (!empty())
    {
        peek().destroy();
        pages.pop();
    }
}

inline bool wci::Navigator::empty() const noexcept
{
    return pages.empty();
}

inline wci::Page& wci::Navigator::peek()
{
    if (empty())
        throw std::logic_error("Navigator is empty");
    return *pages.top();
}

inline void wci::Navigator::push(std::unique_ptr<wci::Page> page)
{
    if (!empty())
        peek().hide();
    pages.push(std::move(page));
    peek().build();
    peek().show();
}

inline void wci::Navigator::pop()
{
    if (empty())
        throw std::logic_error("Navigator is empty");
    peek().hide();
    peek().destroy();
    pages.pop();
    if (!empty())
        peek().show();
}

inline void wci::Navigator::replace(std::unique_ptr<wci::Page> page)
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

inline void wci::Navigator::requestPush(std::unique_ptr<wci::Page> page)
{
    requests.push(wci::Navigator::Request{ wci::Navigator::Action::Push, std::move(page) });
}

inline void wci::Navigator::requestPop()
{
    requests.push(wci::Navigator::Request{ wci::Navigator::Action::Pop });
}

inline void wci::Navigator::requestReplace(std::unique_ptr<wci::Page> page)
{
    requests.push(wci::Navigator::Request{ wci::Navigator::Action::Replace, std::move(page) });
}

inline void wci::Navigator::satisfy()
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

inline bool wci::Navigator::dispatchTick()
{
    bool handled = false;
    if (!empty())
    {
        handled = peek().onTick();
        satisfy();
    }
    return handled;
}

inline bool wci::Navigator::dispatchKeyEvent(const wci::KeyEvent& event)
{
    bool handled = false;
    if (!empty())
    {
        handled = peek().onKeyEvent(event);
        satisfy();
    }
    return handled;
}

inline bool wci::Navigator::dispatchMouseEvent(const wci::MouseEvent& event)
{
    bool handled = false;
    if (!empty())
    {
        handled = peek().onMouseEvent(event);
        satisfy();
    }
    return handled;
}

inline bool wci::Navigator::dispatchResizeEvent(const wci::WindowBufferResizeEvent& event)
{
    bool handled = false;
    if (!empty())
    {
        handled = peek().onResizeEvent(event);
        satisfy();
    }
    return handled;
}

#endif  // WCI_SOURCE_UTIL_NAVIGATOR_HPP
