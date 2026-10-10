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
    private:
        struct {
            bool visible;
        } inner;
        std::function<CharMatrix()> renderer;
        std::function<Geometry()> sizer;

        // idea make constexpr

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

        bool visible() const noexcept;
        void visible(bool newVisibility) noexcept;
    };
}

inline wci::DisplayAdapter::DisplayAdapter(std::function<wci::CharMatrix()> renderer, std::function<wci::Geometry()> sizer) :
    inner{ true },
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

inline bool wci::DisplayAdapter::visible() const noexcept
{
    return inner.visible;
}
inline void wci::DisplayAdapter::visible(bool newVisibility) noexcept
{
    inner.visible = newVisibility;
}

#endif  // WCI_SOURCE_DEFS_DISPLAYADAPTER_HPP
