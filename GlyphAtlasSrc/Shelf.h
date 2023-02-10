//
// Created by Abai on 10.02.2023.
//

#ifndef GLYPHATLAS_SHELF_H
#define GLYPHATLAS_SHELF_H

#include <vector>
#include "Rect.h"

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

#endif //GLYPHATLAS_SHELF_H
