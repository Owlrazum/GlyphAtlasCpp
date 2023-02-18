#include "Shelf.h"

/// Adds into a least possible slot
bool Shelf::TryAdd(std::pair<GlyphKey, Glyph> &glyph)
{
    Rect& rect = glyph.second.rect;
    for (auto & freeSlot : freeSlots)
    {
        ushort width = freeSlot.y - freeSlot.x + 1;
        if (rect.w < width)
        {
            rect.x = freeSlot.x;
            rect.y = shelfPos.y;
            SplitSlot(freeSlot, rect.w, glyph.first);
            return true;
        }
    }

    return false; // The shelfRect width too large to fit into freeSlots of a shelf
}

void Shelf::SplitSlot(Pair &pair, ushort splitWidth, const GlyphKey &key)
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

    ushort x2 = pair.x + slotWidth - 1;
    usedSlots.insert(std::make_pair(key, Pair{pair.x, x2}));
    pair.x = pair.x + slotWidth;
//    if (slotWidth > pair.w)
//    {
//        pair.Update(pair.x + splitWidth, pair.y, pair.w - splitWidth, pair.h);
//    }
}

const std::vector<Pair> &Shelf::GetFreeSlots() const
{
    return freeSlots;
}

bool Shelf::TryRemove(const GlyphKey &key)
{
    if (auto search = usedSlots.find(key); search != usedSlots.end())
    {
        Pair& newFreeSlot = search->second;
        bool isMerged = MergeFreeSlot(newFreeSlot);
        usedSlots.erase(search);
        if (!isMerged)
        {
            freeSlots.emplace_back(newFreeSlot);
        }
        return true;
    }
    return false;
}

bool Shelf::MergeFreeSlot(Pair &newFreeSlot)
{
    int mergedIndex = -1;
    bool isByX = false;
    for (int i = 0; i < freeSlots.size(); i++)
    {
        if (abs(newFreeSlot.x - freeSlots[i].y) == 1)
        {
            freeSlots[i].y = newFreeSlot.y;
            mergedIndex = i;
            isByX = true;
            break;
        }
        if (abs(newFreeSlot.y - freeSlots[i].x) == 1)
        {
            freeSlots[i].x = newFreeSlot.x;
            mergedIndex = i;
            break;
        }
    }

    if (mergedIndex >= 0)
    {
        if (isByX)
        {
            for (int i = 0; i < freeSlots.size(); i++)
            {
                if (abs(freeSlots[mergedIndex].y - freeSlots[i].x) == 1)
                {
                    freeSlots[mergedIndex].y = freeSlots[i].y;
                    //assert(freeSlots[mergedIndex].y > freeSlots[mergedIndex].x);
                    freeSlots.begin() + i = freeSlots.end() - 1; // swap and erase last
                    freeSlots.erase(freeSlots.end() - 1);
                    return true;
                }
            }
        }
        else
        {
            for (int i = 0; i < freeSlots.size(); i++)
            {
                if (abs(freeSlots[mergedIndex].x - freeSlots[i].y) == 1)
                {
                    freeSlots[mergedIndex].x = freeSlots[i].x;
                    //assert(freeSlots[mergedIndex].y > freeSlots[mergedIndex].x);
                    freeSlots.begin() + i = freeSlots.end() - 1;
                    freeSlots.erase(freeSlots.end() - 1);
                    return true;
                }
            }
        }
        return true;
    }
    return false;
}
