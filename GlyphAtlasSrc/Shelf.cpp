#include "Shelf.h"

/// Adds into a least possible slot
bool Shelf::TryAdd(Glyph &glyph)
{
    for (auto & freeSlot : freeSlots)
    {
        if (glyph.rect.w < freeSlot.w)
        {
            glyph.rect.x = freeSlot.x;
            glyph.rect.y = freeSlot.y;
            SplitSlot(freeSlot, glyph.rect.w);
            return true;
        }
    }

    return false; // The shelfRect width too large to fit into freeSlots of a shelf
}

void Shelf::SplitSlot(Rect &rect, ushort splitWidth)
{
    ushort slotWidth = -1;
    for (ushort widthDelimiter : widthDelimiters)
    {
        if (splitWidth < widthDelimiter)
        {
            slotWidth = widthDelimiter;
            break;
        }
    }

    if (slotWidth < 0)
    {
        throw std::out_of_range("Slot split width is too large for provided width delimiters");
    }

    if (slotWidth > rect.w)
    {
        rect.Update(rect.x + splitWidth, rect.y, rect.w - splitWidth, rect.h);
    }
    else
    {
        rect.Update(rect.x + slotWidth, rect.y, rect.w - slotWidth, rect.h); // ushort overflow was here outside of if :)
    }
}

const std::vector<Rect> &Shelf::GetFreeSlots() const
{
    return freeSlots;
}