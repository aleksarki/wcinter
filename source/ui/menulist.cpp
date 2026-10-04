#include <stdexcept>

#include "menulist.hpp"

wci::MenuList::MenuList(
    std::vector<CharString> options,
    bool cyclic,
    wci::Attribute selectionAttribute,
    wci::CharCell backgroundCharacter,
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
        selectionAttribute, backgroundCharacter, optionJustify,
        optionPaddingLeft, optionPaddingRight,
        selectionCharLeft, selectionCharRight,
        showTipPrev, showTipNext,
        tipCharPrev, tipCharNext
    },
    bufferCache{ wci::CharMatrix(), true },
    sizeCache{ wci::Point{}, true }
{}

const wci::CharMatrix& wci::MenuList::toMatrix() const
{
    if (!bufferCache.dirty)
        return bufferCache.buffer;

    auto dimensions = size();
    wci::Point center{ dimensions.x / 2, dimensions.y / 2 };

    if (bufferCache.buffer.size() != dimensions)
        bufferCache.buffer.resize(dimensions, backgroundCharacter());
    else
        bufferCache.buffer.fill(backgroundCharacter());

    if (dimensions.x == 0 || dimensions.y == 0)
    {
        bufferCache.dirty = false;
        return bufferCache.buffer;
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

    wci::Short listWidth = dimensions.x - listStart.x;
    if (selectionCharRight())
        listWidth -= 2;

    for (wci::Short i = 0; i < count(); ++i)  // draw the list of options
    {
        const auto& option = options()[i];
        wci::CharString line(listWidth, backgroundCharacter());
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
            // todo implement CharString::attribute(Attribute)
            for (auto& character : currentOption)
                character.attributes = selectionAttribute();
            // todo implement CharCell ::operator+(Attribute), ::operator+=(Attribute)
            line = wci::CharString(listWidth, wci::CharCell{ backgroundCharacter().character, selectionAttribute() });
            line.inlay(optionStart, currentOption);

            if (selectionCharLeft())
                bufferCache.buffer.put(listStart.x - 2, listStart.y + i, *selectionCharLeft());
            if (selectionCharRight())
                bufferCache.buffer.put(listStart.x + listWidth + 1, listStart.y + i, *selectionCharRight());
        }
        else
            line.inlay(optionStart, option);
        // todo implement Matrix::put(CharString)
        bufferCache.buffer.inlay(wci::Point{ listStart.x, wci::Short(listStart.y + i) }, line.toMatrix());
    }

    if (showTipPrev() && tipCharPrev())  // draw tips
    {
        bufferCache.buffer.put(center.x, 0, wci::CharCell{ wci::chars::upwardsArrow, backgroundCharacter().attributes });
        bufferCache.buffer.put(center.x, 1, *tipCharPrev());
    }
    else if (showTipPrev())
        bufferCache.buffer.put(center.x, 0, wci::CharCell{ wci::chars::upwardsArrow, backgroundCharacter().attributes });
    else if (tipCharPrev())
        bufferCache.buffer.put(center.x, 0, *tipCharPrev());

    if (showTipNext() && tipCharNext())
    {
        bufferCache.buffer.put(center.x, dimensions.y - 2, *tipCharNext());
        bufferCache.buffer.put(center.x, dimensions.y - 1, wci::CharCell{ wci::chars::downwardsArrow, backgroundCharacter().attributes });
    }
    else if (showTipNext())
        bufferCache.buffer.put(center.x, dimensions.y - 1, wci::CharCell{ wci::chars::downwardsArrow, backgroundCharacter().attributes });
    else if (tipCharNext())
        bufferCache.buffer.put(center.x, dimensions.y - 1, *tipCharNext());

    bufferCache.dirty = false;
    return bufferCache.buffer;
}

wci::Point wci::MenuList::size() const
{
    if (!sizeCache.dirty)
        return sizeCache.size;

    sizeCache.size = wci::Point{ 0, 0 };
    for (const auto& option : options())
    {
        sizeCache.size.x = std::max(sizeCache.size.x, option.size());  // find the longest option
        ++sizeCache.size.y;
    }
    sizeCache.size.x += optionPaddingLeft() + optionPaddingRight();
    if (selectionCharLeft())
        sizeCache.size.x += 2;
    if (selectionCharRight())
        sizeCache.size.x += 2;
    if (showTipPrev())
        sizeCache.size.y += 2;
    if (tipCharPrev())
        sizeCache.size.y += 2;
    if (showTipPrev() && tipCharPrev())
        --sizeCache.size.y;
    if (showTipNext())
        sizeCache.size.y += 2;
    if (tipCharNext())
        sizeCache.size.y += 2;
    if (showTipNext() && tipCharNext())
        --sizeCache.size.y;

    sizeCache.dirty = false;
    return sizeCache.size;
}

const std::vector<wci::CharString>& wci::MenuList::options() const noexcept
{
    return inner.options;
}

wci::Short wci::MenuList::addOption(const wci::CharString& option)
{
    inner.options.push_back(option);  // we make a copy
    sizeCache.dirty = true;
    bufferCache.dirty = true;
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
    sizeCache.dirty = true;
    bufferCache.dirty = true;
    return index;
}

wci::CharString& wci::MenuList::getOption(wci::Short index)
{
    if (index < 0)
        throw std::invalid_argument("wci::MenuList::getOption() got invalid value");
    if (index > count() - 1)
        throw std::out_of_range("wci::MenuList::getOption() got value out of range");
    sizeCache.dirty = true;
    bufferCache.dirty = true;
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
    sizeCache.dirty = true;
    bufferCache.dirty = true;
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
    bufferCache.dirty = true;
}

wci::Attribute wci::MenuList::selectionAttribute() const noexcept
{
    return inner.selectionAttribute;
}

void wci::MenuList::selectionAttribute(wci::Attribute newSelectionAttribute) noexcept
{
    inner.selectionAttribute = newSelectionAttribute;
    bufferCache.dirty = true;
}

wci::CharCell wci::MenuList::backgroundCharacter() const noexcept
{
    return inner.backgroundCharacter;
}

void wci::MenuList::backgroundCharacter(wci::CharCell newBackgroundCharacter) noexcept
{
    inner.backgroundCharacter = newBackgroundCharacter;
    bufferCache.dirty = true;
}

wci::Justify wci::MenuList::optionJustify() const noexcept
{
    return inner.optionJustify;
}

void wci::MenuList::optionJustify(wci::Justify newOptionJustify) noexcept
{
    inner.optionJustify = newOptionJustify;
    bufferCache.dirty = true;
}

unsigned wci::MenuList::optionPaddingLeft() const noexcept
{
    return inner.optionPaddingLeft;
}

void wci::MenuList::optionPaddingLeft(unsigned newOptionPaddingLeft) noexcept
{
    inner.optionPaddingLeft = newOptionPaddingLeft;
    sizeCache.dirty = true;
    bufferCache.dirty = true;
}

unsigned wci::MenuList::optionPaddingRight() const noexcept
{
    return inner.optionPaddingRight;
}

void wci::MenuList::optionPaddingRight(unsigned newOptionPaddingRight) noexcept
{
    inner.optionPaddingRight = newOptionPaddingRight;
    sizeCache.dirty = true;
    bufferCache.dirty = true;
}

const std::optional<wci::CharCell>& wci::MenuList::selectionCharLeft() const noexcept
{
    return inner.selectionCharLeft;
}

void wci::MenuList::selectionCharLeft(const std::optional<wci::CharCell>& newSelectionCharLeft)
{
    inner.selectionCharLeft = newSelectionCharLeft;
    sizeCache.dirty = true;
    bufferCache.dirty = true;
}

const std::optional<wci::CharCell>& wci::MenuList::selectionCharRight() const noexcept
{
    return inner.selectionCharRight;
}

void wci::MenuList::selectionCharRight(const std::optional<wci::CharCell>& newSelectionCharRight)
{
    inner.selectionCharRight = newSelectionCharRight;
    sizeCache.dirty = true;
    bufferCache.dirty = true;
}

bool wci::MenuList::showTipPrev() const noexcept
{
    return inner.showTipPrev;
}

void wci::MenuList::showTipPrev(bool newShowTipPrev) noexcept
{
    inner.showTipPrev = newShowTipPrev;
    sizeCache.dirty = true;
    bufferCache.dirty = true;
}

bool wci::MenuList::showTipNext() const noexcept
{
    return inner.showTipNext;
}

void wci::MenuList::showTipNext(bool newShowTipNext) noexcept
{
    inner.showTipNext = newShowTipNext;
    sizeCache.dirty = true;
    bufferCache.dirty = true;
}

const std::optional<wci::CharCell>& wci::MenuList::tipCharPrev() const noexcept
{
    return inner.tipCharPrev;
}

void wci::MenuList::tipCharPrev(const std::optional<wci::CharCell>& newTipCharPrev)
{
    inner.tipCharPrev = newTipCharPrev;
    sizeCache.dirty = true;
    bufferCache.dirty = true;
}

const std::optional<wci::CharCell>& wci::MenuList::tipCharNext() const noexcept
{
    return inner.tipCharNext;
}

void wci::MenuList::tipCharNext(const std::optional<wci::CharCell>& newTipCharNext)
{
    inner.tipCharNext = newTipCharNext;
    sizeCache.dirty = true;
    bufferCache.dirty = true;
}

bool wci::MenuList::selected() const noexcept
{
    return index() != wci::noindex;
}

void wci::MenuList::deselect() noexcept
{
    inner.index = wci::noindex;
    bufferCache.dirty = true;
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
    bufferCache.dirty = true;
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
    bufferCache.dirty = true;
    return index();
}
