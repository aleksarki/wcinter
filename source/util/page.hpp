#pragma once
#ifndef WCI_SOURCE_UTIL_PAGE_HPP
#define WCI_SOURCE_UTIL_PAGE_HPP

#include <concepts>
#include <cstddef>
#include <utility>
#include <vector>

#include "../core.hpp"
#include "../defs.hpp"

namespace wci
{
    class Page
    {
    protected:
        Window& window;
        std::vector<std::size_t> ids;

        template<typename... Args>
        std::size_t place(Args&&... args)
        {
            auto id = window.place(std::forward<Args>(args)...);
            ids.push_back(id);
            return id;
        }

    public:
        Page(Window& window);
        virtual ~Page();

        Page(const Page&) = delete;
        Page& operator=(const Page& page) = delete;

        virtual void build();

        virtual void destroy();

        virtual void show();

        virtual void hide();

        virtual bool onTick();

        virtual bool onKeyEvent(const KeyEvent& event);

        virtual bool onMouseEvent(const MouseEvent& event);

        virtual bool onResizeEvent(const WindowBufferResizeEvent& event);
    };

    template<typename P>
    concept page = std::derived_from<P, Page>;
}

inline wci::Page::Page(wci::Window& window) : window(window)
{}

inline wci::Page::~Page() = default;

inline void wci::Page::build()
{}

inline void wci::Page::destroy()
{
    for (auto id : ids)
        window.unplace(id);
    ids.clear();
}

inline void wci::Page::show()
{
    for (auto id : ids)
        window.adapter(id).visible(true);
}

inline void wci::Page::hide()
{
    for (auto id : ids)
        window.adapter(id).visible(false);
}

inline bool wci::Page::onTick()  // assume by default that tick is handled
{
    return true;
}

inline bool wci::Page::onKeyEvent(const wci::KeyEvent& event)
{
    return false;
}

inline bool wci::Page::onMouseEvent(const wci::MouseEvent& event)
{
    return false;
}

inline bool wci::Page::onResizeEvent(const wci::WindowBufferResizeEvent& event)
{
    return false;
}

#endif  // WCI_SOURCE_UTIL_PAGE_HPP
