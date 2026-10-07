#pragma once
#ifndef WCI_SOURCE_DEFS_DISPLAYADAPTER_HPP
#define WCI_SOURCE_DEFS_DISPLAYADAPTER_HPP

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

    /*
     * Concept to make sure object can be rendered onto window.
     * We can render anything that can produce char matrix of itself and tell its size.
     */
    template<typename T>
    concept renderable = requires(const T& t)
    {
        { t.toMatrix() } -> std::convertible_to<CharMatrix>;
        { t.size() } -> std::convertible_to<Geometry>;
    };

    // NB: if widget constructs from an lvalue-object, the object must outlive the widget!
    /*
     * A display adapter helps place objects on screen, move them and render them.
     */
    class DisplayAdapter
    {
        friend class Window;

    private:
        struct {
            PositionSpec position;
            Anchor anchor;
            Point absolute;  // calculated position of the top left point
            bool visible;
            std::variant<std::monostate, Window*, DisplayAdapter*> parent;
            std::size_t id;  // wci::npos if widget is not placed
        } inner;
        std::function<CharMatrix()> renderer;
        std::function<Geometry()> sizer;

        // ctor for delegating and filling inner struct with default values
        DisplayAdapter(std::function<CharMatrix()> renderer, std::function<Geometry()> sizer);

    public:
        template<renderable R>
        DisplayAdapter(R& object);
        template<renderable R>
        DisplayAdapter(R&& object);
        DisplayAdapter(Wchar character);
        DisplayAdapter(const CharCell& cell);
        DisplayAdapter(CharCell&& cell);
        DisplayAdapter(const std::wstring& string);
        DisplayAdapter(std::wstring&& string);
        DisplayAdapter(const CharString& charString);
        DisplayAdapter(CharString&& charString);
        DisplayAdapter(CharMatrix&& charMatrix);

        DisplayAdapter(const DisplayAdapter&) = delete;
        DisplayAdapter& operator=(const DisplayAdapter&) = delete;

        DisplayAdapter(DisplayAdapter&&) = default;
        DisplayAdapter& operator=(DisplayAdapter&&) = default;

        Geometry size() const noexcept;

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

        bool parentIsAdapter() const noexcept;

        Window& window() const;  // should check prior whether has parent window

        Window* windowp() const;  // get nullptr if does not have parent window
    private:
        void windowp(Window* newWindowPtr) noexcept;

    public:
        DisplayAdapter& parent() const;  // should check prior whether has parent widget

        DisplayAdapter* parentp() const;  // get nullptr if does not have parent widget
    private:
        void parentp(DisplayAdapter* newParentPtr) noexcept;

        void unplace() noexcept;

        // todo in displayadapter.cpp:
        // todo make position(), anchor() and unplace() public through Window
        // todo implement const std::vector<wci::DisplayAdapter*>& children() const
    };
}

inline wci::DisplayAdapter::DisplayAdapter(std::function<wci::CharMatrix()> renderer, std::function<wci::Geometry()> sizer) :
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
inline wci::DisplayAdapter::DisplayAdapter(R& object) : wci::DisplayAdapter(
    [&object]() -> wci::CharMatrix { return object.toMatrix(); },
    [&object]() -> wci::Geometry { return object.size(); }
)
{}
template<wci::renderable R>
inline wci::DisplayAdapter::DisplayAdapter(R&& object) : wci::DisplayAdapter(
    [matrix = object.toMatrix()]() -> wci::CharMatrix { return matrix; },
    [size = object.size()]() -> wci::Geometry { return size; }
)
{}
inline wci::DisplayAdapter::DisplayAdapter(wci::Wchar character) : wci::DisplayAdapter(
    [matrix = wci::CharMatrix(wci::Geometry{ 1, 1 }, wci::CharCell{ character, wci::normal })]() -> wci::CharMatrix { return matrix; },
    [size = wci::Geometry{ 1, 1 }]() -> wci::Geometry { return size; }
)
{}
inline wci::DisplayAdapter::DisplayAdapter(const CharCell& cell) : wci::DisplayAdapter(
    [&cell]() -> wci::CharMatrix { return wci::CharMatrix(wci::Geometry{ 1, 1 }, cell); },
    [size = wci::Geometry{ 1, 1 }]() -> wci::Geometry { return size; }
)
{}
inline wci::DisplayAdapter::DisplayAdapter(wci::CharCell&& cell) : wci::DisplayAdapter(
    [matrix = wci::CharMatrix(wci::Geometry{ 1, 1 }, cell)]() -> wci::CharMatrix { return matrix; },
    [size = wci::Geometry{ 1, 1 }]() -> wci::Geometry{ return size; }
)
{}
inline wci::DisplayAdapter::DisplayAdapter(const std::wstring& string) : wci::DisplayAdapter(
    [&string]() -> wci::CharMatrix { return wci::CharString(string).toMatrix(); },
    [&string]() -> wci::Geometry { return wci::Geometry{ static_cast<wci::Short>(string.size()), 1 }; }
)
{}
inline wci::DisplayAdapter::DisplayAdapter(std::wstring&& string) : wci::DisplayAdapter(
    [matrix = wci::CharString(string).toMatrix()]() -> wci::CharMatrix { return matrix; },
    [size = wci::Geometry{ wci::CharString(string).size(), 1 }]() -> wci::Geometry{ return size; }
)
{}
inline wci::DisplayAdapter::DisplayAdapter(const CharString& charString) : wci::DisplayAdapter (
    [&charString]() -> wci::CharMatrix { return charString.toMatrix(); },
    [&charString]() -> wci::Geometry { return wci::Geometry{ charString.size(), 1 }; }
)
{}
inline wci::DisplayAdapter::DisplayAdapter(CharString&& charString) : wci::DisplayAdapter(
    [matrix = charString.toMatrix()]() -> wci::CharMatrix { return matrix; },
    [size = wci::Geometry{ charString.size(), 1 }]() -> wci::Geometry { return size; }
)
{}
inline wci::DisplayAdapter::DisplayAdapter(CharMatrix&& charMatrix) : wci::DisplayAdapter(
    [charMatrix]() -> wci::CharMatrix { return charMatrix; },
    [size = charMatrix.size()]() -> wci::Geometry { return size; }
)
{}

inline wci::Geometry wci::DisplayAdapter::size() const noexcept
{
    return sizer();
}

inline wci::CharMatrix wci::DisplayAdapter::render() const
{
    return renderer();
}

inline const wci::PositionSpec& wci::DisplayAdapter::position() const noexcept
{
    return inner.position;
}
inline void wci::DisplayAdapter::position(const wci::PositionSpec& newPosition) noexcept
{
    inner.position = newPosition;
}

inline wci::Anchor wci::DisplayAdapter::anchor() const noexcept
{
    return inner.anchor;
}
inline void wci::DisplayAdapter::anchor(wci::Anchor newAnchor) noexcept
{
    inner.anchor = newAnchor;
}

inline wci::Point wci::DisplayAdapter::absolute() const noexcept
{
    return inner.absolute;
}
inline void wci::DisplayAdapter::absolute(wci::Point newAbsolute) noexcept
{
    inner.absolute = newAbsolute;
}

inline std::size_t wci::DisplayAdapter::id() const noexcept
{
    return inner.id;
}

inline void wci::DisplayAdapter::id(std::size_t newId) noexcept
{
    inner.id = newId;
}

inline bool wci::DisplayAdapter::visible() const noexcept
{
    return inner.visible;
}
inline void wci::DisplayAdapter::visible(bool newVisibility) noexcept
{
    inner.visible = newVisibility;
}

inline bool wci::DisplayAdapter::placed() const noexcept
{
    return !std::holds_alternative<std::monostate>(inner.parent);
}

inline bool wci::DisplayAdapter::parentIsWindow() const noexcept
{
    return std::holds_alternative<wci::Window*>(inner.parent);
}

inline bool wci::DisplayAdapter::parentIsAdapter() const noexcept
{
    return std::holds_alternative<wci::DisplayAdapter*>(inner.parent);
}

inline wci::Window& wci::DisplayAdapter::window() const
{
    return *std::get<wci::Window*>(inner.parent);  // wonderful that this works
}

inline wci::Window* wci::DisplayAdapter::windowp() const
{
    if (parentIsWindow())
        return std::get<wci::Window*>(inner.parent);
    return nullptr;
}
inline void wci::DisplayAdapter::windowp(wci::Window* newWindow) noexcept
{
    assert(newWindow != nullptr);
    inner.parent = newWindow;
}

inline wci::DisplayAdapter& wci::DisplayAdapter::parent() const
{
    return *std::get<wci::DisplayAdapter*>(inner.parent);
}

inline wci::DisplayAdapter* wci::DisplayAdapter::parentp() const
{
    if (parentIsAdapter())
        return std::get<wci::DisplayAdapter*>(inner.parent);
    return nullptr;
}
inline void wci::DisplayAdapter::parentp(wci::DisplayAdapter* newParent) noexcept
{
    assert(newParent != nullptr);
    inner.parent = newParent;
}

inline void wci::DisplayAdapter::unplace() noexcept
{
    inner.parent = std::monostate{};
    inner.id = wci::npos;
}

#endif  // WCI_SOURCE_DEFS_DISPLAYADAPTER_HPP
