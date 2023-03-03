#include "Shelf.h"

#include "EndPoints.h"
#include <iostream>

/// Adds into a least possible slot
bool Shelf::TryAdd(std::pair<GlyphKey, Glyph> &glyph, uint16 slotWidth)
{
    Rect& rect = glyph.second.rect;
    for (auto & freeSlot : freeSlots)
    {
        uint16 width = freeSlot.y - freeSlot.x + 1;
        if (slotWidth < width)
        {
            rect.x = freeSlot.x;
            rect.y = crossEndPoints.x;
            SplitSlot(freeSlot, slotWidth, glyph.first);
            return true;
        }
    }

    return false; // The shelfRect width too large to fit into freeSlots of a shelf
}

void Shelf::SplitSlot(uint2_16 &freeSlot, uint16 slotWidth, const GlyphKey &key)
{
    uint16 x2 = freeSlot.x + slotWidth - 1;
    assert(freeSlot.x < x2);
    usedSlots.insert(std::make_pair(key, uint2_16{freeSlot.x, x2}));
    freeSlot.x = freeSlot.x + slotWidth;
    assert(freeSlot.x < freeSlot.y);
}

const std::vector<uint2_16> &Shelf::GetFreeSlots() const
{
    return freeSlots;
}

std::pair<bool, bool> Shelf::TryRemove(const GlyphKey &key)
{
    if (auto search = usedSlots.find(key); search != usedSlots.end())
    {
        if (usedSlots.size() == 1) // the shelf will become empty and get deleted, can return here
        {
            return std::make_pair(true, true);
        }

        uint2_16& newFreeSlot = search->second;
        assert(newFreeSlot.y > newFreeSlot.x);
        ClaimFreeSlot(newFreeSlot);

        usedSlots.erase(search);
        return std::make_pair(true, false);
    }
    return std::make_pair(false, false);
}

void Shelf::ClaimFreeSlot(uint2_16 &newFreeSlot)
{
    bool isMerged = MergeIntoIfPossible(newFreeSlot, freeSlots);
    if (!isMerged)
    {
        freeSlots.emplace_back(newFreeSlot);
    }
}
