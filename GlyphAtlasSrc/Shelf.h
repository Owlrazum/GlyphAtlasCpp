#pragma once

#include "Rect.h"
#include "Glyph.h"
#include "GlyphKey.h"
#include <map>
#include <vector>

class Shelf
{
public:
    Shelf(uint2_16 mainEndPointsArg, uint2_16 crossEndPointsArg, uint2_16& minMaxSizeArg)
      : mainEndPoints(mainEndPointsArg),
      crossEndPoints(crossEndPointsArg),
      minMaxSize(minMaxSizeArg),
      usedSlots()
    {
        freeSlots = {mainEndPoints}; // free slot on the whole of the shelf's size
    }

    // naming using main and cross similar to flex-box, not tied to vertical or horizontal.
    uint2_16 mainEndPoints; // poses of start and end, endPoints.y > endPoints.x
    uint2_16 crossEndPoints;
    uint2_16 minMaxSize; // used to not place too short inside shelf.

    bool TryAdd(std::pair<GlyphKey, Glyph> &glyph, uint16 slotWidth);
    [[nodiscard]] const std::vector<uint2_16>& GetFreeSlots() const;
    std::pair<bool, bool> TryRemove(const GlyphKey &key); // returns first whether key was removed, second whether the shelf is empty
private:
    std::map<GlyphKey, uint2_16> usedSlots;
    std::vector<uint2_16> freeSlots; // pair here is two positions inclusive, start and end on x-axis.

    void SplitSlot(uint2_16 &freeSlot, uint16 slotWidth, const GlyphKey &key);
    void ClaimFreeSlot(uint2_16 &newFreeSlot); // returns whether slot was merged
};