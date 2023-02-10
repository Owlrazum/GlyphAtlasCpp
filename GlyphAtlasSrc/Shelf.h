#pragma once

#include "Rect.h"
#include <vector>

class Shelf
{
public:
    Shelf(Rect& rectArg, const std::vector<ushort> widthDelimitersArg)
    : rect(rectArg), widthDelimiters(widthDelimitersArg)
    {
        freeSlots = {rect};
    }

    Rect rect;

    bool TryAdd(Rect& rect);
private:
    std::vector<Rect> freeSlots;
    const std::vector<ushort> widthDelimiters;

    void SplitSlot(Rect& rect, ushort splitWidth);
};