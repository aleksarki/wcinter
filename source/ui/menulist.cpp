#include <stdexcept>

#include "menulist.hpp"
#include "../util.hpp"

wci::MenuList::MenuList(
    std::vector<CharString> options,
    bool cyclic,
    wci::Attribute selectionAttribute,
    wci::CharCell backgroundCell,
    wci::Justify optionJustify,
    unsigned optionPaddingLeft,
    unsigned optionPaddingRight,
    std::optional<wci::CharCell> selectionCharLeft,
    std::optional<wci::CharCell> selectionCharRight,
    bool showTipPrev,
    bool showTipNext,
    std::optional<wci::CharCell> tipCharPrev,
    std::optional<wci::CharCell> tipCharNext
) :
    inner{
        std::move(options), cyclic, wci::noindex,
        selectionAttribute, backgroundCell, optionJustify,
        optionPaddingLeft, optionPaddingRight,
        selectionCharLeft, selectionCharRight,
        showTipPrev, showTipNext,
        tipCharPrev, tipCharNext
    },
    matrixCache(wci::CharMatrix()),
    sizeCache(wci::Geometry{})
{}

const wci::CharMatrix& wci::MenuList::toMatrix() const
{
    if (matrixCache.valid())
        return matrixCache.read();

    auto dimensions = size();
    auto center = dimensions.toPoint() / 2.;

    if (matrixCache.value().size() != dimensions)
        matrixCache.value().resize(dimensions, backgroundCell());
    else
        matrixCache.value().fill(backgroundCell());

    if (dimensions.empty())
    {
        matrixCache.clean();
        return matrixCache.read();
    }

    wci::Point listStart{ 0, 0 };
    if (selectionCharLeft())
        listStart.x += 2;
    if (showTipPrev())
        listStart.y += 2;
    if (tipCharPrev())
        listStart.y += 2;
    if (showTipPrev() && tipCharPrev())
        --listStart.y;

    wci::Short listWidth = dimensions.width - listStart.x;
    if (selectionCharRight())
        listWidth -= 2;

    for (wci::Short i = 0; i < count(); ++i)  // draw the list of options
    {
        const auto& option = options()[i];
        wci::CharString line(listWidth, backgroundCell());
        wci::Short optionLength = option.size();
        wci::Short optionStart;

        switch (optionJustify())
        {
        case wci::Justify::Left:
            optionStart = optionPaddingLeft();
            break;
        case wci::Justify::Center:
            optionStart = (listWidth - optionPaddingLeft() - optionPaddingRight()) / 2 - optionLength / 2 + optionPaddingLeft();
            break;
        case wci::Justify::Right:
            optionStart = listWidth - optionLength - optionPaddingRight();
            break;
        default:
            throw std::invalid_argument("wci::MenuList::toMatrix() got invalid wci::Justify value");
        }

        if (i == index())  // selected option, distinct highlight  // todo OPTIMIZE THIS
        {
            wci::CharString currentOption = option;
            currentOption.attribute(selectionAttribute());
            line = wci::CharString(listWidth, backgroundCell() << selectionAttribute());
            line.inlay(optionStart, currentOption);

            if (selectionCharLeft())
                matrixCache.value().put(listStart + wci::at(-2, i), *selectionCharLeft());
            if (selectionCharRight())
                matrixCache.value().put(listStart + wci::at(listWidth + 1, i), *selectionCharRight());
        }
        else
            line.inlay(optionStart, option);
        // todo implement Matrix::put(CharString)?
        matrixCache.value().inlay(listStart + wci::at(0, i), line.toMatrix());
    }

    if (showTipPrev() && tipCharPrev())  // draw tips
    {
        matrixCache.value().put(center.x, 0, wci::chars::upwardsArrow + backgroundCell().attributes);
        matrixCache.value().put(center.x, 1, *tipCharPrev());
    }
    else if (showTipPrev())
        matrixCache.value().put(center.x, 0, wci::chars::upwardsArrow + backgroundCell().attributes);
    else if (tipCharPrev())
        matrixCache.value().put(center.x, 0, *tipCharPrev());

    if (showTipNext() && tipCharNext())
    {
        matrixCache.value().put(center.x, dimensions.height - 2, *tipCharNext());
        matrixCache.value().put(center.x, dimensions.height - 1, wci::chars::downwardsArrow + backgroundCell().attributes);
    }
    else if (showTipNext())
        matrixCache.value().put(center.x, dimensions.height - 1, wci::chars::downwardsArrow + backgroundCell().attributes);
    else if (tipCharNext())
        matrixCache.value().put(center.x, dimensions.height - 1, *tipCharNext());

    matrixCache.clean();
    return matrixCache.read();
}

wci::Geometry wci::MenuList::size() const
{
    if (sizeCache.valid())
        return sizeCache.read();

    sizeCache.value() = wci::Geometry{ 0, 0 };
    for (const auto& option : options())
    {
        sizeCache.value().width = std::max(sizeCache.value().width, option.size());  // find the longest option
        ++sizeCache.value().height;
    }
    sizeCache.value().width += optionPaddingLeft() + optionPaddingRight();
    if (selectionCharLeft())
        sizeCache.value().width += 2;
    if (selectionCharRight())
        sizeCache.value().width += 2;
    if (showTipPrev())
        sizeCache.value().height += 2;
    if (tipCharPrev())
        sizeCache.value().height += 2;
    if (showTipPrev() && tipCharPrev())
        --sizeCache.value().height;
    if (showTipNext())
        sizeCache.value().height += 2;
    if (tipCharNext())
        sizeCache.value().height += 2;
    if (showTipNext() && tipCharNext())
        --sizeCache.value().height;

    sizeCache.clean();
    return sizeCache.read();
}

const std::vector<wci::CharString>& wci::MenuList::options() const noexcept
{
    return inner.options;
}

wci::Short wci::MenuList::addOption(const wci::CharString& option)
{
    inner.options.push_back(option);  // we make a copy
    sizeCache.dirty();
    matrixCache.dirty();
    return count() - 1;
}

wci::Short wci::MenuList::insertOption(wci::Short index, const wci::CharString& option)
{
    if (index < 0)
        throw std::invalid_argument("wci::MenuList::insertOption() got invalid value");
    if (index > count())
        throw std::out_of_range("wci::MenuList::insertOption() got value out of range");
    auto copy = option;  // we make the copy in advance bc insertion can invalidate option if we got it as a ref from options
    inner.options.insert(options().begin() + index, std::move(copy));
    if (selected() && index <= inner.index)  // insertion before current selection
        ++inner.index;
    sizeCache.dirty();
    matrixCache.dirty();
    return index;
}

wci::CharString& wci::MenuList::getOption(wci::Short index)
{
    if (index < 0)
        throw std::invalid_argument("wci::MenuList::getOption() got invalid value");
    if (index > count() - 1)
        throw std::out_of_range("wci::MenuList::getOption() got value out of range");
    sizeCache.dirty();
    matrixCache.dirty();
    return inner.options[index];
}

const wci::CharString& wci::MenuList::getOption(wci::Short index) const
{
    if (index < 0)
        throw std::invalid_argument("wci::MenuList::getOption() got invalid value");
    if (index > count() - 1)
        throw std::out_of_range("wci::MenuList::getOption() got value out of range");
    return options()[index];
}

void wci::MenuList::removeOption(wci::Short index)
{
    if (index < 0)
        throw std::invalid_argument("wci::MenuList::removeOption() got invalid value");
    if (index > count() - 1)
        throw std::out_of_range("wci::MenuList::removeOption() got value out of range");
    inner.options.erase(options().begin() + index);
    if (selected())
        if (index == inner.index)  // removal of current selection
            inner.index = wci::noindex;
        else if (index < inner.index)  // removal before current selection
            --inner.index;
    sizeCache.dirty();
    matrixCache.dirty();
}

bool wci::MenuList::cyclic() const noexcept
{
    return inner.cyclic;
}

void wci::MenuList::cyclic(bool newCyclic) noexcept
{
    inner.cyclic = newCyclic;
}

wci::Short wci::MenuList::index() const noexcept
{
    return inner.index;
}

void wci::MenuList::index(wci::Short newIndex)
{
    if (newIndex < 0)
        throw std::invalid_argument("wci::MenuList::index() got invalid value");
    if (newIndex >= count())
        throw std::out_of_range("wci::MenuList::index() got value out of range");
    inner.index = newIndex;
    matrixCache.dirty();
}

wci::Attribute wci::MenuList::selectionAttribute() const noexcept
{
    return inner.selectionAttribute;
}

void wci::MenuList::selectionAttribute(wci::Attribute newSelectionAttribute) noexcept
{
    inner.selectionAttribute = newSelectionAttribute;
    matrixCache.dirty();
}

wci::CharCell wci::MenuList::backgroundCell() const noexcept
{
    return inner.backgroundCell;
}

void wci::MenuList::backgroundCell(wci::CharCell newBackgroundCharacter) noexcept
{
    inner.backgroundCell = newBackgroundCharacter;
    matrixCache.dirty();
}

wci::Justify wci::MenuList::optionJustify() const noexcept
{
    return inner.optionJustify;
}

void wci::MenuList::optionJustify(wci::Justify newOptionJustify) noexcept
{
    inner.optionJustify = newOptionJustify;
    matrixCache.dirty();
}

unsigned wci::MenuList::optionPaddingLeft() const noexcept
{
    return inner.optionPaddingLeft;
}

void wci::MenuList::optionPaddingLeft(unsigned newOptionPaddingLeft) noexcept
{
    inner.optionPaddingLeft = newOptionPaddingLeft;
    sizeCache.dirty();
    matrixCache.dirty();
}

unsigned wci::MenuList::optionPaddingRight() const noexcept
{
    return inner.optionPaddingRight;
}

void wci::MenuList::optionPaddingRight(unsigned newOptionPaddingRight) noexcept
{
    inner.optionPaddingRight = newOptionPaddingRight;
    sizeCache.dirty();
    matrixCache.dirty();
}

const std::optional<wci::CharCell>& wci::MenuList::selectionCharLeft() const noexcept
{
    return inner.selectionCharLeft;
}

void wci::MenuList::selectionCharLeft(const std::optional<wci::CharCell>& newSelectionCharLeft)
{
    inner.selectionCharLeft = newSelectionCharLeft;
    sizeCache.dirty();
    matrixCache.dirty();
}

const std::optional<wci::CharCell>& wci::MenuList::selectionCharRight() const noexcept
{
    return inner.selectionCharRight;
}

void wci::MenuList::selectionCharRight(const std::optional<wci::CharCell>& newSelectionCharRight)
{
    inner.selectionCharRight = newSelectionCharRight;
    sizeCache.dirty();
    matrixCache.dirty();
}

bool wci::MenuList::showTipPrev() const noexcept
{
    return inner.showTipPrev;
}

void wci::MenuList::showTipPrev(bool newShowTipPrev) noexcept
{
    inner.showTipPrev = newShowTipPrev;
    sizeCache.dirty();
    matrixCache.dirty();
}

bool wci::MenuList::showTipNext() const noexcept
{
    return inner.showTipNext;
}

void wci::MenuList::showTipNext(bool newShowTipNext) noexcept
{
    inner.showTipNext = newShowTipNext;
    sizeCache.dirty();
    matrixCache.dirty();
}

const std::optional<wci::CharCell>& wci::MenuList::tipCharPrev() const noexcept
{
    return inner.tipCharPrev;
}

void wci::MenuList::tipCharPrev(const std::optional<wci::CharCell>& newTipCharPrev)
{
    inner.tipCharPrev = newTipCharPrev;
    sizeCache.dirty();
    matrixCache.dirty();
}

const std::optional<wci::CharCell>& wci::MenuList::tipCharNext() const noexcept
{
    return inner.tipCharNext;
}

void wci::MenuList::tipCharNext(const std::optional<wci::CharCell>& newTipCharNext)
{
    inner.tipCharNext = newTipCharNext;
    sizeCache.dirty();
    matrixCache.dirty();
}

bool wci::MenuList::selected() const noexcept
{
    return index() != wci::noindex;
}

void wci::MenuList::deselect() noexcept
{
    inner.index = wci::noindex;
    matrixCache.dirty();
}

wci::Short wci::MenuList::count() const noexcept
{
    return static_cast<wci::Short>(options().size());
}

wci::Short wci::MenuList::prev() noexcept
{
    if (!selected())
    {
        if (count() > 0)
            inner.index = 0;
    }
    else if (index() > 0)
        --inner.index;
    else if (cyclic())  // cycle back
        inner.index = count() - 1;
    matrixCache.dirty();
    return index();
}

wci::Short wci::MenuList::next() noexcept
{
    if (!selected())
    {
        if (count() > 0)
            inner.index = 0;
    }
    else if (index() < count() - 1)
        ++inner.index;
    else if(cyclic())  // cycle forward
        inner.index = 0;
    matrixCache.dirty();
    return index();
}
