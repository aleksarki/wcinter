#pragma once
#ifndef WCI_SOURCE_UTIL_MISC_HPP
#define WCI_SOURCE_UTIL_MISC_HPP

#include <cmath>
#include <stdexcept>
#include <type_traits>

#include "../defs.hpp"

namespace wci
{
    /*
     * Define a point.
     */
    constexpr Point at(Short x, Short y) noexcept;

    /*
     * Define a geometry.
     */
    constexpr Geometry size(Short width, Short height) noexcept;

    /*
     * Define a character cell with normal attribute.
     */
    constexpr CharCell cell(Wchar character) noexcept;
    /*
     * Define a character cell.
     */
    constexpr CharCell cell(Wchar character, Attribute attribute) noexcept;

    /*
     * Define a position specification.
     */
    constexpr PositionSpec position(LenPercent x, LenPercent y);

    /*
     * Define an absolute position specification.
     */
    constexpr PositionSpec absolute(Short x, Short y) noexcept;

    /*
     * Define a relative position specification.
     */
    constexpr PositionSpec relative(double relx, double rely);

    /*
     * Get the part of an attribute that defines the foreground.
     */
    constexpr Attribute fg(Attribute attribute) noexcept;

    /*
     * Get the part of an attribute that defines the background.
     */
    constexpr Attribute bg(Attribute attribute) noexcept;

    /*
     * Replace foreground for an attribute.
     */
    constexpr Attribute withFg(Attribute base, Attribute fg) noexcept;

    /*
     * Replace background for an attribute.
     */
    constexpr Attribute withBg(Attribute base, Attribute bg) noexcept;

    /*
     * Swap foreground and background of an attribute.
     */
    constexpr Attribute inversed(Attribute attribute) noexcept;

    /*
     * Calculate absolute position of the top left corner of a rectangle stacked upon another one.
     */
    constexpr Point calculateStackeeStart(
        const PositionSpec& stackeePosition, const Geometry& baseSize, const Geometry& stackeeSize, Anchor stackeeAnchor
    );
}

inline constexpr wci::Point wci::at(wci::Short x, wci::Short y) noexcept
{
    return wci::Point{ x, y };
}

inline constexpr wci::Geometry wci::size(wci::Short width, wci::Short height) noexcept
{
    return wci::Geometry{ width, height };
}

inline constexpr wci::CharCell wci::cell(Wchar character) noexcept
{
    return wci::CharCell{ character, wci::normal };
}
inline constexpr wci::CharCell wci::cell(wci::Wchar character, wci::Attribute attribute) noexcept
{
    return wci::CharCell{ character, attribute };
}

inline constexpr wci::PositionSpec wci::position(wci::LenPercent x, wci::LenPercent y)
{
    return wci::PositionSpec{ x, y };
}

inline constexpr wci::PositionSpec wci::absolute(wci::Short x, wci::Short y) noexcept
{
    return wci::PositionSpec{ wci::LenPercent(x), wci::LenPercent(y) };
}

inline constexpr wci::PositionSpec wci::relative(double relx, double rely)
{
    return wci::PositionSpec{ wci::LenPercent(relx), wci::LenPercent(rely) };
}

inline constexpr wci::Attribute wci::fg(wci::Attribute attribute) noexcept
{
    return attribute & wci::Attribute::FgColorWhiteBright;
}

inline constexpr wci::Attribute wci::bg(wci::Attribute attribute) noexcept
{
    return attribute & wci::Attribute::BgColorWhiteBright;
}

inline constexpr wci::Attribute wci::withFg(wci::Attribute base, wci::Attribute fg) noexcept
{
    return base & ~wci::Attribute::FgColorWhiteBright | wci::fg(fg);
}

inline constexpr wci::Attribute wci::withBg(wci::Attribute base, wci::Attribute bg) noexcept
{
    return base & ~wci::Attribute::BgColorWhiteBright | wci::bg(bg);
}

inline constexpr wci::Attribute wci::inversed(wci::Attribute attribute) noexcept
{
    using U = std::underlying_type_t<wci::Attribute>;
    auto newFg = static_cast<wci::Attribute>(static_cast<U>(wci::bg(attribute)) >> 4);
    auto newBg = static_cast<wci::Attribute>(static_cast<U>(wci::fg(attribute)) << 4);
    return attribute & ~wci::Attribute::FgColorWhiteBright & ~wci::Attribute::BgColorWhiteBright | newFg | newBg;
}

inline constexpr wci::Point wci::calculateStackeeStart(
    const wci::PositionSpec &stackeePosition, const wci::Geometry& baseSize, const wci::Geometry& stackeeSize, wci::Anchor stackeeAnchor
)
{
    wci::Point anchor{ 0, 0 };  // init just in case?
    wci::Point half = stackeeSize.toPoint() / 2.;
    wci::Point size = stackeeSize.toPoint();

    if (stackeePosition.x.isAbsolute())
        anchor.x = stackeePosition.x.absolute();
    else if (stackeePosition.x.isRelative())
        anchor.x = static_cast<wci::Short>(std::lround(stackeePosition.x.relative() * baseSize.width));

    if (stackeePosition.y.isAbsolute())
        anchor.y = stackeePosition.y.absolute();
    else if (stackeePosition.y.isRelative())
        anchor.y = static_cast<wci::Short>(std::lround(stackeePosition.y.relative() * baseSize.height));

    switch (stackeeAnchor)
    {
    case wci::Anchor::TopLeft:     return anchor;
    case wci::Anchor::Top:         return anchor - wci::at(half.x, 0);
    case wci::Anchor::TopRight:    return anchor - wci::at(size.x, 0);
    case wci::Anchor::Right:       return anchor - wci::at(size.x, half.y);
    case wci::Anchor::BottomRight: return anchor - size;
    case wci::Anchor::Bottom:      return anchor - wci::at(half.x, size.y);
    case wci::Anchor::BottomLeft:  return anchor - wci::at(0, size.y);
    case wci::Anchor::Left:        return anchor - wci::at(0, half.y);
    case wci::Anchor::Center:      return anchor - half;
    }

    throw std::invalid_argument("wci::calculateStackeeStart() got invalid Anchor value");
}

#endif  // WCI_SOURCE_UTIL_MISC_HPP
