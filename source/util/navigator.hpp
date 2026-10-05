#pragma once
#ifndef WCI_SOURCE_UTIL_NAVIGATOR_HPP
#define WCI_SOURCE_UTIL_NAVIGATOR_HPP

#include <concepts>
#include <memory>
#include <queue>
#include <stack>
#include <utility>

#include "../defs.hpp"

namespace wci
{
    class Page;

    template<typename P>
    concept page = std::derived_from<P, Page>;

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

#endif  // WCI_SOURCE_UTIL_NAVIGATOR_HPP
