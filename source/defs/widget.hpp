#pragma once
#ifndef WCI_SOURCE_DEFS_WIDGET_HPP
#define WCI_SOURCE_DEFS_WIDGET_HPP

#include <cassert>
#include <cstddef>
#include <concepts>
#include <functional>
#include <string>
#include <variant>

#include "charmatrix.hpp"
#include "charstring.hpp"
#include "consts.hpp"
#include "enums.hpp"
#include "lenpercent.hpp"
#include "structs.hpp"
#include "types.hpp"

namespace wci
{
    class Window;

    template<typename T>
    concept renderable = requires(const T& t)
    {
        { t.toMatrix() } -> std::convertible_to<CharMatrix>;
        { t.size() } -> std::convertible_to<Point>;
    };

    // NB: if widget constructs from an lvalue-object, the object must outlive the widget!
    class Widget
    {
        friend class Window;

    private:
        struct {
            PositionSpec position;
            Anchor anchor;
            Point absolute;  // calculated position of the top left point
            bool visible;
            std::variant<std::monostate, Window*, Widget*> parent;
            std::size_t id;  // wci::npos if widget is not placed
        } inner;
        std::function<CharMatrix()> renderer;
        std::function<Point()> sizer;

        // ctor for delegating and filling inner struct with default values
        Widget(std::function<CharMatrix()> renderer, std::function<Point()> sizer);

    public:
        template<renderable R>
        Widget(R& object);
        Widget(Wchar character);
        Widget(const CharCell& cell);
        Widget(CharCell&& cell);
        Widget(const std::wstring& string);
        Widget(std::wstring&& string);
        Widget(const CharString& charString);
        Widget(CharString&& charString);
        Widget(CharMatrix&& charMatrix);

        Widget(const Widget&) = delete;
        Widget& operator=(const Widget&) = delete;

        Widget(Widget&&) = default;
        Widget& operator=(Widget&&) = default;

        Point size() const noexcept;

        CharMatrix render() const;  // this does not render its children

        const PositionSpec& position() const noexcept;  // absolute or relative position of anchor upon parent
    private:
        void position(const PositionSpec& newPosition) noexcept;

    public:
        Anchor anchor() const noexcept;  // part of the widget positioning upon parent is relative to
    private:
        void anchor(Anchor newAnchor) noexcept;

    public:
        Point absolute() const noexcept;  // calculated absolute coords of top left corner
    private:
        void absolute(Point newAbsolute) noexcept;

    public:
        std::size_t id() const noexcept;
    private:
        void id(std::size_t newId) noexcept;

    public:
        bool visible() const noexcept;
        void visible(bool newVisibility) noexcept;

        bool placed() const noexcept;  // has been placed upon either window or another widget

        bool parentIsWindow() const noexcept;

        bool parentIsWidget() const noexcept;

        Window& window() const;  // should check prior whether has parent window

        Window* windowp() const;  // get nullptr if does not have parent window
    private:
        void windowp(Window* newWindowPtr) noexcept;

    public:
        Widget& parent() const;  // should check prior whether has parent widget

        Widget* parentp() const;  // get nullptr if does not have parent widget
    private:
        void parentp(Widget* newParentPtr) noexcept;

        void unplace() noexcept;

        // todo in widget.cpp:
        // todo make position(), anchor() and unplace() public through Window
        // todo implement const std::vector<wci::Widget*>& children() const
    };
}

inline wci::Widget::Widget(std::function<wci::CharMatrix()> renderer, std::function<wci::Point()> sizer) :
    inner{
        wci::PositionSpec{ wci::LenPercent(wci::Short(0)), wci::LenPercent(wci::Short(0)) },
        wci::Anchor::TopLeft,
        wci::Point{ 0, 0 },
        true,
        std::monostate{},
        wci::npos
    },
    renderer(std::move(renderer)),
    sizer(std::move(sizer))
{}
template<wci::renderable R>
inline wci::Widget::Widget(R& object) : wci::Widget(
    [&object]() -> wci::CharMatrix { return object.toMatrix(); },
    [&object]() -> wci::Point { return object.size(); }
)
{}
inline wci::Widget::Widget(wci::Wchar character) : wci::Widget(
    [matrix = wci::CharMatrix(wci::Point{ 1, 1 }, wci::CharCell{ character, wci::normal })]() -> wci::CharMatrix { return matrix; },
    [size = wci::Point{ 1, 1 }]() -> wci::Point { return size; }
)
{}
inline wci::Widget::Widget(const CharCell& cell) : wci::Widget(
    [&cell]() -> wci::CharMatrix { return wci::CharMatrix(wci::Point{ 1, 1 }, cell); },
    [size = wci::Point{ 1, 1 }]() -> wci::Point { return size; }
)
{}
inline wci::Widget::Widget(wci::CharCell&& cell) : wci::Widget(
    [matrix = wci::CharMatrix(wci::Point{ 1, 1 }, cell)]() -> wci::CharMatrix { return matrix; },
    [size = wci::Point{ 1, 1 }]() -> wci::Point { return size; }
)
{}
inline wci::Widget::Widget(const std::wstring& string) : wci::Widget(
    [&string]() -> wci::CharMatrix { return wci::CharString(string).toMatrix(); },
    [&string]() -> wci::Point { return wci::Point{ wci::CharString(string).size(), 1 }; }
)
{}
inline wci::Widget::Widget(std::wstring&& string) : wci::Widget(
    [matrix = wci::CharString(string).toMatrix()]() -> wci::CharMatrix { return matrix; },
    [size = wci::Point{ wci::CharString(string).size(), 1 }]() -> wci::Point { return size; }
)
{}
inline wci::Widget::Widget(const CharString& charString) : wci::Widget (
    [&charString]() -> wci::CharMatrix { return charString.toMatrix(); },
    [&charString]() -> wci::Point { return wci::Point{ charString.size(), 1 }; }
)
{}
inline wci::Widget::Widget(CharString&& charString) : wci::Widget(
    [matrix = charString.toMatrix()]() -> wci::CharMatrix { return matrix; },
    [size = wci::Point{ charString.size(), 1 }]() -> wci::Point { return size; }
)
{}
inline wci::Widget::Widget(CharMatrix&& charMatrix) : wci::Widget(
    [charMatrix]() -> wci::CharMatrix { return charMatrix; },
    [size = charMatrix.size()]() -> wci::Point { return size; }
)
{}

inline wci::Point wci::Widget::size() const noexcept
{
    return sizer();
}

inline wci::CharMatrix wci::Widget::render() const
{
    return renderer();
}

inline const wci::PositionSpec& wci::Widget::position() const noexcept
{
    return inner.position;
}
inline void wci::Widget::position(const wci::PositionSpec& newPosition) noexcept
{
    inner.position = newPosition;
}

inline wci::Anchor wci::Widget::anchor() const noexcept
{
    return inner.anchor;
}
inline void wci::Widget::anchor(wci::Anchor newAnchor) noexcept
{
    inner.anchor = newAnchor;
}

inline wci::Point wci::Widget::absolute() const noexcept
{
    return inner.absolute;
}
inline void wci::Widget::absolute(wci::Point newAbsolute) noexcept
{
    inner.absolute = newAbsolute;
}

inline std::size_t wci::Widget::id() const noexcept
{
    return inner.id;
}

inline void wci::Widget::id(std::size_t newId) noexcept
{
    inner.id = newId;
}

inline bool wci::Widget::visible() const noexcept
{
    return inner.visible;
}
inline void wci::Widget::visible(bool newVisibility) noexcept
{
    inner.visible = newVisibility;
}

inline bool wci::Widget::placed() const noexcept
{
    return !std::holds_alternative<std::monostate>(inner.parent);
}

inline bool wci::Widget::parentIsWindow() const noexcept
{
    return std::holds_alternative<wci::Window*>(inner.parent);
}

inline bool wci::Widget::parentIsWidget() const noexcept
{
    return std::holds_alternative<wci::Widget*>(inner.parent);
}

inline wci::Window& wci::Widget::window() const
{
    return *std::get<wci::Window*>(inner.parent);  // wonderful that this works
}

inline wci::Window* wci::Widget::windowp() const
{
    if (parentIsWindow())
        return std::get<wci::Window*>(inner.parent);
    return nullptr;
}
inline void wci::Widget::windowp(wci::Window* newWindow) noexcept
{
    assert(newWindow != nullptr);
    inner.parent = newWindow;
}

inline wci::Widget& wci::Widget::parent() const
{
    return *std::get<wci::Widget*>(inner.parent);
}

inline wci::Widget* wci::Widget::parentp() const
{
    if (parentIsWidget())
        return std::get<wci::Widget*>(inner.parent);
    return nullptr;
}
inline void wci::Widget::parentp(wci::Widget* newParent) noexcept
{
    assert(newParent != nullptr);
    inner.parent = newParent;
}

inline void wci::Widget::unplace() noexcept
{
    inner.parent = std::monostate{};
    inner.id = wci::npos;
}

#endif  // WCI_SOURCE_DEFS_WIDGET_HPP
