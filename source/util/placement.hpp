#pragma once
#ifndef WCI_SOURCE_UTIL_PLACEMENT_HPP
#define WCI_SOURCE_UTIL_PLACEMENT_HPP

#include <cmath>
#include <concepts>
#include <stdexcept>

#include "../defs.hpp"

namespace wci
{
    // todo move the contents

    template<typename M>
    concept measurable = requires(M m)
    {
        { m.size() } -> std::convertible_to<Geometry>;
    };

    template<measurable P>
    struct Placement
    {
        P* object;
        PositionSpec position;
        Anchor anchor;
        int z;

        /*
        * Calculate the absolute position of the top left corner on the parent.
        */
        Point absolute(const Geometry& parentSize) const;
    };
}

template <wci::measurable P>
inline wci::Point wci::Placement<P>::absolute(const wci::Geometry& parentSize) const
{
    wci::Point start{ 0, 0 };  // init just in case?
    wci::Point size = object->size().toPoint();
    wci::Point half = size / 2.;

    if (position.x.isAbsolute())
        start.x = position.x.absolute();
    else if (position.x.isRelative())
        start.x = static_cast<wci::Short>(std::lround(position.x.relative() * parentSize.width));

    if (position.y.isAbsolute())
        start.y = position.y.absolute();
    else if (position.y.isRelative())
        start.y = static_cast<wci::Short>(std::lround(position.y.relative() * parentSize.height));

    switch (anchor)
    {
        case wci::Anchor::TopLeft:     return start;
        case wci::Anchor::Top:         return start - wci::at(half.x, 0);
        case wci::Anchor::TopRight:    return start - wci::at(size.x, 0);
        case wci::Anchor::Right:       return start - wci::at(size.x, half.y);
        case wci::Anchor::BottomRight: return start - size;
        case wci::Anchor::Bottom:      return start - wci::at(half.x, size.y);
        case wci::Anchor::BottomLeft:  return start - wci::at(0, size.y);
        case wci::Anchor::Left:        return start - wci::at(0, half.y);
        case wci::Anchor::Center:      return start - half;
    }

    throw std::invalid_argument("wci::Placementer::calculatePlacementStart() got invalid Anchor value");
}

#endif  // WCI_SOURCE_UTIL_PLACEMENT_HPP
