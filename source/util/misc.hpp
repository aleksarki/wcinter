#pragma once
#ifndef WCI_SOURCE_UTIL_MISC_HPP
#define WCI_SOURCE_UTIL_MISC_HPP

#include "../defs.hpp"

namespace wci
{
    /*
     * Define a point.
     */
    inline constexpr Coord at(Short x, Short y) noexcept;

    /*
     * Calculate absolute position of the top left corner of a rectangle stacked upon another one.
     */
    inline constexpr Coord calculateStackeeStart(const PositionSpec& stackeePosition, Coord baseSize, Coord stackeeSize, Anchor stackeeAnchor) noexcept;
}

inline constexpr wci::Coord wci::at(wci::Short x, wci::Short y) noexcept
{
    return wci::Coord{ x, y };
}

inline constexpr wci::Coord wci::calculateStackeeStart(
    const wci::PositionSpec &stackeePosition, wci::Coord baseSize, wci::Coord stackeeSize, wci::Anchor stackeeAnchor
) noexcept
{
    wci::Coord anchorPoint{ 0, 0 }, startPoint{ 0, 0 };  // init just in case?

    if (stackeePosition.x.isAbsolute())
        anchorPoint.x = stackeePosition.x.absolute();
    else if (stackeePosition.x.isRelative())
        anchorPoint.x = static_cast<wci::Short>(std::lround(stackeePosition.x.relative() * baseSize.x));

    if (stackeePosition.y.isAbsolute())
        anchorPoint.y = stackeePosition.y.absolute();
    else if (stackeePosition.y.isRelative())
        anchorPoint.y = static_cast<wci::Short>(std::lround(stackeePosition.y.relative() * baseSize.y));

    switch (stackeeAnchor)
    {
    case wci::Anchor::TopLeft:
        startPoint = anchorPoint;
        break;
    case wci::Anchor::Top:
        startPoint.x = anchorPoint.x - stackeeSize.x / 2;
        startPoint.y = anchorPoint.y;
        break;
    case wci::Anchor::TopRight:
        startPoint.x = anchorPoint.x - stackeeSize.x;
        startPoint.y = anchorPoint.y;
        break;
    case wci::Anchor::Right:
        startPoint.x = anchorPoint.x - stackeeSize.x;
        startPoint.y = anchorPoint.y - stackeeSize.y / 2;
        break;
    case wci::Anchor::BottomRight:
        startPoint.x = anchorPoint.x - stackeeSize.x;
        startPoint.y = anchorPoint.y - stackeeSize.y;
        break;
    case wci::Anchor::Bottom:
        startPoint.x = anchorPoint.x - stackeeSize.x / 2;
        startPoint.y = anchorPoint.y - stackeeSize.y;
        break;
    case wci::Anchor::BottomLeft:
        startPoint.x = anchorPoint.x;
        startPoint.y = anchorPoint.y - stackeeSize.y;
        break;
    case wci::Anchor::Left:
        startPoint.x = anchorPoint.x;
        startPoint.y = anchorPoint.y - stackeeSize.y / 2;
        break;
    case wci::Anchor::Center:
        startPoint.x = anchorPoint.x - stackeeSize.x / 2;
        startPoint.y = anchorPoint.y - stackeeSize.y / 2;
        break;
    }

    return startPoint;
}

#endif  // WCI_SOURCE_UTIL_MISC_HPP
