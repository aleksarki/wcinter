#pragma once
#ifndef WCI_SOURCE_UI_MENULIST_HPP
#define WCI_SOURCE_UI_MENULIST_HPP

#include <optional>
#include <vector>

#include "../defs.hpp"

namespace wci
{
    class MenuList
    {
    protected:
        struct {
            std::vector<CharString> options;
            bool cyclic;                                                    // do cyclic rotation through options
            Short index;
            Attribute selectionAttribute;
            CharCell backgroundCharacter;
            Justify optionJustify;
            unsigned optionPaddingLeft, optionPaddingRight;
            std::optional<CharCell> selectionCharLeft, selectionCharRight;  // these embrace current selection
            bool showTipPrev, showTipNext;
            std::optional<CharCell> tipCharPrev, tipCharNext;               // these are visual tips for switching options
        } inner;
        mutable struct {
            CharMatrix buffer;
            bool dirty;
        } bufferCache;
        mutable struct {
            Geometry size;
            bool dirty;
        } sizeCache;

    public:
        MenuList(
            std::vector<CharString> options = std::vector<CharString>(),
            bool cyclic = true,
            Attribute selectionAttribute = rnormal,
            CharCell backgroundCharacter = normalch,
            Justify optionJustify = Justify::Left,
            unsigned optionPaddingLeft = 1,
            unsigned optionPaddingRight = 1,
            std::optional<CharCell> selectionCharLeft = std::nullopt,
            std::optional<CharCell> selectionCharRight = std::nullopt,
            bool showTipPrev = false, bool showTipNext = false,
            std::optional<CharCell> tipCharPrev = std::nullopt,
            std::optional<CharCell> tipCharNext = std::nullopt
        );

        const CharMatrix& toMatrix() const;

        Geometry size() const;

        const std::vector<CharString>& options() const noexcept;

        Short addOption(const CharString& option);

        Short insertOption(Short index, const CharString& option);

        CharString& getOption(Short index);
        const CharString& getOption(Short index) const;

        void removeOption(Short index);

        bool cyclic() const noexcept;
        void cyclic(bool newCyclic) noexcept;

        Short index() const noexcept;
        void index(Short newIndex);

        Attribute selectionAttribute() const noexcept;
        void selectionAttribute(Attribute newSelectionAttribute) noexcept;

        CharCell backgroundCharacter() const noexcept;
        void backgroundCharacter(CharCell newBackgroundCharacter) noexcept;

        Justify optionJustify() const noexcept;
        void optionJustify(Justify newOptionJustify) noexcept;

        unsigned optionPaddingLeft() const noexcept;
        void optionPaddingLeft(unsigned newOptionPaddingLeft) noexcept;

        unsigned optionPaddingRight() const noexcept;
        void optionPaddingRight(unsigned newOptionPaddingRight) noexcept;

        const std::optional<CharCell>& selectionCharLeft() const noexcept;
        void selectionCharLeft(const std::optional<CharCell>& newSelectionCharLeft);

        const std::optional<CharCell>& selectionCharRight() const noexcept;
        void selectionCharRight(const std::optional<CharCell>& newSelectionCharRight);

        bool showTipPrev() const noexcept;
        void showTipPrev(bool newShowTipPrev) noexcept;

        bool showTipNext() const noexcept;
        void showTipNext(bool newShowTipNext) noexcept;

        const std::optional<CharCell>& tipCharPrev() const noexcept;
        void tipCharPrev(const std::optional<CharCell>& newTipCharPrev);

        const std::optional<CharCell>& tipCharNext() const noexcept;
        void tipCharNext(const std::optional<CharCell>& newTipCharNext);

        bool selected() const noexcept;

        void deselect() noexcept;

        Short count() const noexcept;

        Short prev() noexcept;

        Short next() noexcept;
    };
}

#endif  // WCI_SOURCE_UI_MENULIST_HPP
