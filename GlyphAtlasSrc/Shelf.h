#pragma once

#include "Rect.h"
#include "Glyph.h"
#include "GlyphKey.h"
#include <map>
#include <vector>

class Shelf
{
public:
    Shelf(Pair mainEndPointsArg, Pair crossEndPointsArg, Pair& minMaxSizeArg)
      : mainEndPoints(mainEndPointsArg),
      crossEndPoints(crossEndPointsArg),
      minMaxSize(minMaxSizeArg),
      usedSlots()
    {
        freeSlots = {mainEndPoints}; // free slot on the whole of the shelf's size
    }

    // naming using main and cross similar to flex-box, not tied to vertical or horizontal.
    Pair mainEndPoints; // poses of start and end, endPoints.y > endPoints.x
    Pair crossEndPoints;
    Pair minMaxSize; // used to not place too short inside shelf.

    bool TryAdd(std::pair<GlyphKey, Glyph> &glyph, ushort slotWidth);
    [[nodiscard]] const std::vector<Pair>& GetFreeSlots() const;
    std::pair<bool, bool> TryRemove(const GlyphKey &key); // returns first whether key was removed, second whether the shelf is empty
private:
    std::map<GlyphKey, Pair> usedSlots;
    std::vector<Pair> freeSlots; // pair here is two positions inclusive, start and end on x-axis.

    void SplitSlot(Pair &freeSlot, ushort slotWidth, const GlyphKey &key);
    void ClaimFreeSlot(Pair &newFreeSlot); // returns whether slot was merged
};