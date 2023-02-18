#pragma once

#include "Rect.h"
#include "Glyph.h"
#include "GlyphKey.h"
#include <map>
#include <vector>

class Shelf
{
public:
    Shelf(Pair& posArg, ushort width, Pair& minMaxHeightArg, const std::vector<ushort>& widthDelimitersArg)
    : minMaxHeight(minMaxHeightArg),
      shelfPos(posArg),
      widthDelimiters(widthDelimitersArg),
      usedSlots()
    {
        freeSlots = {{shelfPos.x, static_cast<ushort>(shelfPos.x + width - 1)}};
    }

    Pair shelfPos;
    Pair minMaxHeight; // used to not place too short inside shelf.

    bool TryAdd(std::pair<GlyphKey, Glyph> &glyph);
    [[nodiscard]] const std::vector<Pair>& GetFreeSlots() const;
    bool TryRemove(const GlyphKey &key);
private:
    std::map<GlyphKey, Pair> usedSlots;
    std::vector<Pair> freeSlots; // pair here is two positions inclusive, start and end on x axis.
    const std::vector<ushort> widthDelimiters; // should be sorted in ascending order.

    void SplitSlot(Pair &pair, ushort splitWidth, const GlyphKey &key);
    bool MergeFreeSlot(Pair &newFreeSlot); // returns whether slot was merged
};