#include "Shelf.h"

#include "Algorithms.h"

/// Adds into a least possible slot
bool Shelf::TryAdd(std::pair<GlyphKey, Glyph> &glyph, uint16 slotWidth)
{
    Rect& rect = glyph.second.rect;
    for (machine i = 0; i < freeSlots.size(); i++)
    {
        uint16 freeWidth = freeSlots[i].y - freeSlots[i].x + 1;
        if (slotWidth <= freeWidth)
        {
            rect.x = freeSlots[i].x;
            rect.y = crossEndPoints.x;
            if (slotWidth != freeWidth)
            {
                uint16_2 toReduce = freeSlots[i];
                SplitSlot(toReduce, slotWidth, glyph.first);
                freeSlots[i] = toReduce;
            }
            else
            {
                usedSlots.insert(std::make_pair(glyph.first, freeSlots[i]));
                freeSlots.erase(freeSlots.begin() + i);
            }

            CheckIntegrity();
            return true;
        }
    }

    return false; // The shelfRect width too large to fit into freeSlots of a shelf
}

std::pair<bool, bool> Shelf:: TryRemove(const GlyphKey &key)
{
    if (auto search = usedSlots.find(key); search != usedSlots.end())
    {
        if (usedSlots.size() == 1) // the shelf will become empty and get deleted, can return here
        {
            return std::make_pair(true, true);
        }

        CheckIntegrity();

        uint16_2 newFreeSlot = search->second;
        usedSlots.erase(search);
        ClaimFreeSlot(newFreeSlot);

        CheckIntegrity();
        return std::make_pair(true, false);
    }
    return std::make_pair(false, false);
}

void Shelf::ClaimFreeSlot(uint16_2 &newFreeSlot)
{
    bool isMerged = MergeIntoIfPossibleDebug(newFreeSlot, freeSlots, usedSlots, mainEndPoints.y + 1);
    if (!isMerged)
    {
        freeSlots.emplace_back(newFreeSlot);
    }
}

void Shelf::SplitSlot(uint16_2 &freeSlot, uint16 slotWidth, const GlyphKey &key)
{
    uint16_2 usedSlot(freeSlot.x, freeSlot.x + slotWidth - 1);
    assert(usedSlot.x <= usedSlot.y);
    usedSlots.insert(std::make_pair(key, usedSlot));
    freeSlot.x = freeSlot.x + slotWidth;
    assert(freeSlot.x <= freeSlot.y);
    assert(usedSlot.x != freeSlot.x);
}

void Shelf::CheckIntegrity()
{
    std::vector<uint16_2> slots;
    slots.reserve((usedSlots.size() + freeSlots.size()));
    for (auto freeSlot : freeSlots)
    {
        slots.push_back(freeSlot);
    }
    for (auto usedSlot : usedSlots)
    {
        slots.push_back(usedSlot.second);
    }

    CheckContainerIntegrity(slots, mainEndPoints.y + 1);
}

const std::vector<uint16_2> &Shelf::GetFreeSlots() const
{
    return freeSlots;
}