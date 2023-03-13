#pragma once

#include "Rect.h"
#include "Glyph.h"
#include "GlyphKey.h"
#include <map>
#include <vector>

class Shelf
{
public:
    Shelf(uint16_2 mainEndPointsArg, uint16_2 crossEndPointsArg, uint16_2& minMaxSizeArg)
      : mainEndPoints(mainEndPointsArg),
      crossEndPoints(crossEndPointsArg),
      minMaxSize(minMaxSizeArg),
      usedSlots()
    {
        freeSlots = {mainEndPoints}; // free slot on the whole of the shelf's size
    }

    // naming using main and cross similar to flex-box, not tied to vertical or horizontal.
    uint16_2 mainEndPoints; // poses of start and end, endPoints.y > endPoints.x
    uint16_2 crossEndPoints;
    uint16_2 minMaxSize; // used to not place too short inside shelf.

    bool TryAdd(std::pair<GlyphKey, Glyph> &glyph, uint16 slotWidth);
    [[nodiscard]] const std::vector<uint16_2>& GetFreeSlots() const;
    std::pair<bool, bool> TryRemove(const GlyphKey &key); // returns first whether key was removed, second whether the shelf is empty

    void CheckIntegrity();
private:
    std::map<GlyphKey, uint16_2> usedSlots;
    std::vector<uint16_2> freeSlots; // pair here is two positions inclusive, start and end on x-axis.

    void SplitSlot(uint16_2 &freeSlot, uint16 slotWidth, const GlyphKey &key);
    void ClaimFreeSlot(uint16_2 &newFreeSlot); // returns whether slot was merged
};