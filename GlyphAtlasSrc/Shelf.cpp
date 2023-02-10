//
// Created by Abai on 10.02.2023.
//

#include "Shelf.h"

/// Adds into a least possible slot
bool Shelf::TryAdd(Rect &rect)
{
    for (int i = 0; i < freeSlots.size(); i++)
    {
        if (rect.w < freeSlots[i].w)
        {
            rect.x = freeSlots[i].x;
            rect.y = freeSlots[i].y;
            SplitSlot(freeSlots[i], rect.w);
            return true;
        }
    }

    return false; // The rect width too large to fit into freeSlots of a shelf
}

void Shelf::SplitSlot(Rect &rect, ushort splitWidth)
{
    ushort slotWidth = -1;
    for (int i = 0; i < widthDelimiters.size(); i++)
    {
        if (splitWidth < widthDelimiters[i])
        {
            slotWidth = widthDelimiters[i];
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
        rect.Update(rect.x + slotWidth, rect.y, rect.w - slotWidth, rect.h); // ushort overflow was here outside of if)
    }
}
