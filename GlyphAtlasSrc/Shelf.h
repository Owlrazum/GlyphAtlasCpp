#pragma once

#include "Rect.h"
#include "Glyph.h"
#include <vector>

class Shelf
{
public:
    Shelf(Pair& pos, ushort width, Pair& minMaxHeightArg, const std::vector<ushort>& widthDelimitersArg)
    : minMaxHeight(minMaxHeightArg), widthDelimiters(widthDelimitersArg)
    {
        freeSlots = {{pos.x, pos.y, width, minMaxHeightArg.y}};
    }

    Pair minMaxHeight; // used to not place too short inside shelf.

    bool TryAdd(Glyph& glyph);
    const std::vector<Rect>& GetFreeSlots() const;
private:
    std::vector<Rect> freeSlots;
    const std::vector<ushort> widthDelimiters;

    void SplitSlot(Rect& rect, ushort splitWidth);
};