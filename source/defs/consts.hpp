#pragma once
#ifndef WCI_SOURCE_DEFS_CONSTS_HPP
#define WCI_SOURCE_DEFS_CONSTS_HPP

#include <cstddef>
#include <cstdint>

#include "charcell.hpp"
#include "enums.hpp"
#include "types.hpp"

namespace wci
{
    inline constexpr struct Reset {} reset;

    inline constexpr Attribute normal = Attribute::BgColorBlack | Attribute::FgColorWhite;
    inline constexpr Attribute rnormal = Attribute::BgColorWhite | Attribute::FgColorBlack;

    inline constexpr Wchar nline = L'\n';
    inline constexpr Wchar tab = L'\t';
    inline constexpr Wchar zero = L'\0';

    inline constexpr CharCell normalch{ zero, normal };
    inline constexpr CharCell nullch{ zero, Attribute::No };

    inline constexpr Id npos(-1);

    inline constexpr std::uint8_t true8 = true;
    inline constexpr std::uint8_t false8 = false;

    namespace chars
    {
        inline constexpr Wchar
            invertedExclamation = L'¡',
            cent = L'¢',
            pound = L'£',
            currency = L'¤',
            yen = L'¥',
            brokenBar = L'¦',
            section = L'§',
            copyright = L'©',
            leftPointingDoubleAngleQuotation = L'«',
            notSign = L'¬',
            registered = L'®',
            degree = L'°',
            plusMinus = L'±',
            superscriptTwo = L'²',
            superscriptThree = L'³',
            micro = L'µ',
            pilcrow = L'¶',
            middleDot = L'·',
            superscriptOne = L'¹',
            rightPointingDoubleAngleQuotation = L'»',
            vulgarFractionOneQuarter = L'¼',
            vulgarFractionOneHalf = L'½',
            vulgarFractionThreeHalves = L'¾',
            invertedQuestion = L'¿',
            multiplication = L'×',
            division = L'÷',
            leftSingleQuotation = L'‘',
            rightSingleQuotation = L'’',
            singleLow9Quotation = L'‚',
            singleHighReversed9Quotation = L'‛',
            leftDoubleQuotation = L'“',
            rightDoubleQuotation = L'”',
            doubleLow9Quotation = L'„',
            doubleHighReversed9QuotationMark = L'‟',
            dagger = L'†',
            doubleDagger = L'‡',
            bullet = L'•',
            triangularBullet = L'‣',
            horizontalEllipsis = L'…',
            perMille = L'‰',
            perTenThousandSign = L'‱',
            caret = L'‸',
            singleLeftPointingAngleQuotation = L'‹',
            singleRightPointingAngleQuotation = L'›',
            reference = L'※',
            doubleExclamation = L'‼',
            interrobang = L'‽',
            doubleQuestion = L'⁇',
            questionExclamation = L'⁈',
            exclamationQuestion = L'⁉',
            superscriptZero = L'⁰',
            superscriptFour = L'⁴',
            superscriptFive = L'⁵',
            superscriptSix = L'⁶',
            superscriptSeven = L'⁷',
            superscriptEight = L'⁸',
            superscriptNine = L'⁹',
            subscriptZero = L'₀',
            subscriptOne = L'₁',
            subscriptTwo = L'₂',
            subscriptThree = L'₃',
            subscriptFour = L'₄',
            subscriptFive = L'₅',
            subscriptSix = L'₆',
            subscriptSeven = L'₇',
            subscriptEight = L'₈',
            subscriptNine = L'₉',
            numero = L'№',
            leftwardsArrow = L'←',
            upwardsArrow = L'↑',
            rightwardsArrow = L'→',
            downwardsArrow = L'↓',
            leftRightArrow = L'↔',
            upDownArrow = L'↕',
            upDownArrowWithBase = L'↨',
            partialDifferential = L'∂',
            increment = L'∆',
            nabla = L'∇',
            nAryProduct = L'∏',
            nArySummation = L'∑',
            squareRoot = L'√',
            infinity = L'∞',
            intersection = L'∩',
            integral = L'∫',
            asymptoticallyEqualTo = L'≃',
            almostEqualTo = L'≈',
            notEqualTo = L'≠',
            identicalTo = L'≡',
            lessThanOrEqualTo = L'≤',
            greaterThanOrEqualTo = L'≥',
            diameter = L'⌀',
            house = L'⌂',
            lightHorizontal = L'─',
            lightVertical = L'│',
            lightDownRight = L'┌',
            lightDownLeft = L'┐',
            lightUpRight = L'└',
            lightUpLeft = L'┘',
            lightVerticalRight = L'├',
            lightVerticalLeft = L'┤',
            lightDownHorizontal = L'┬',
            lightUpHorizontal = L'┴',
            lightVerticalHorizontal = L'┼',
            doubleHorizontal = L'═',
            doubleVertical = L'║',
            downSingleRightDouble = L'╒',
            downDoubleRightSingle = L'╓',
            doubleDownRight = L'╔',
            downSingleLeftDouble = L'╕',
            downDoubleLeftSingle = L'╖',
            doubleDownLeft = L'╗',
            upSingleRightDouble = L'╘',
            upDoubleRightSingle = L'╙',
            doubleUpRight = L'╚',
            upSingleLeftDouble = L'╛',
            upDoubleLeftSingle = L'╜',
            doubleUpLeft = L'╝',
            verticalSingleRightDouble = L'╞',
            verticalDoubleRightSingle = L'╟',
            doubleVerticalRight = L'╠',
            verticalSingleLeftDouble = L'╡',
            verticalDoubleLeftSingle = L'╢',
            doubleVerticalLeft = L'╣',
            downSingleHorizontalDouble = L'╤',
            downDoubleHorizontalSingle = L'╥',
            doubleDownHorizontal = L'╦',
            upSingleHorizontalDouble = L'╧',
            upDoubleHorizontalSingle = L'╨',
            doubleUpHorizontal = L'╩',
            verticalSingleHorizontalDouble = L'╪',
            verticalDoubleHorizontalSingle = L'╫',
            doubleVerticalHorizontal = L'╬',
            upperHalfBlock = L'▀',
            lowerHalfBlock = L'▄',
            fullBlock = L'█',
            leftHalfBlock = L'▌',
            rightHalfBlock = L'▐',
            lightShade = L'░',
            mediumShade = L'▒',
            darkShade = L'▓',
            whiteSmilingFace = L'☺',
            blackSmilingFace = L'☻',
            female = L'♀',
            male = L'♂',
            blackSpadeSuit = L'♠',
            blackClubSuit = L'♣',
            blackHeartSuit = L'♥',
            blackDiamondSuit = L'♦',
            eighthNote = L'♪',
            beamedEighthNotes = L'♫';
    }
}

#endif  // WCI_SOURCE_DEFS_CONSTS_HPP
