#include "GlyphTextureStepped.h"

#include "Algorithms.h"
#include <algorithm>
#include <iterator>
#include <iostream>

bool GlyphTextureStepped::UpdateStep(std::pair<GlyphKey, Glyph> toPlace)
{
    GlyphKey &glyphKey = toPlace.first;
    Glyph &glyph = toPlace.second;

    uint16 slotWidth = 0;
    for (uint16 widthDelimiter : widthDelimiters.get())
    {
        if (glyph.rect.w <= widthDelimiter)
        {
            slotWidth = widthDelimiter;
            break;
        }
    }
    if (slotWidth == 0)
    {
        throw std::out_of_range("The shelfRect width too large to fit into just created shelf");
    }

    if (ContainsGlyph(glyphKey))
    {
        glyph.textureId = id;
        currentlyPlacedGlyphs.insert(glyphKey);
        return true;
    }

    bool found = FitInExistingSpot(toPlace, slotWidth);
    if (found)
    {
        placedGlyphs.insert(toPlace);
        currentlyPlacedGlyphs.insert(glyphKey);
        return true;
    }

    bool wasPlaced = CreateShelf(toPlace, slotWidth);
    if (wasPlaced)
    {
        placedGlyphs.insert(toPlace);
        currentlyPlacedGlyphs.insert(glyphKey);
        return true;
    }

    return false;
}

std::vector<GlyphKey> GlyphTextureStepped::CollectUnused()
{
    std::vector<GlyphKey> unusedGlyphs;
    if (previouslyPlacedGlyphs.empty())
    {
        previouslyPlacedGlyphs = currentlyPlacedGlyphs;
        currentlyPlacedGlyphs.clear();
        return unusedGlyphs;
    }

    std::set_difference(previouslyPlacedGlyphs.begin(), previouslyPlacedGlyphs.end(),
                        currentlyPlacedGlyphs.begin(), currentlyPlacedGlyphs.end(),
                        std::inserter(unusedGlyphs, unusedGlyphs.begin()));

    previouslyPlacedGlyphs = currentlyPlacedGlyphs;
    currentlyPlacedGlyphs.clear();

    return unusedGlyphs;
}

CRect GlyphTextureStepped::RemoveStep(GlyphKey &keyToRemove)
{
    if (placedGlyphs.find(keyToRemove) == placedGlyphs.end())
    {
        throw std::out_of_range("The key is not placed inside this texture!");
    }

    for (machine i = 0; i < shelves.size(); i++)
    {
        auto keyRemovedShelfEmpty = shelves[i].TryRemove(keyToRemove);
        if (keyRemovedShelfEmpty.first)
        {
            if (keyRemovedShelfEmpty.second)
            {
                RemoveShelf(shelves.begin() + i);
            }
            break;
        }
    }
    CRect rect = placedGlyphs[keyToRemove].rect;

    placedGlyphs.erase(keyToRemove);
    return rect;
}

std::map<GlyphKey, Glyph>& GlyphTextureStepped::GetGlyphs()
{
    return placedGlyphs;
}

std::pair<std::vector<Rect>, std::vector<Rect>> GlyphTextureStepped::GetFreeShelfSlotSpace() const
{
    std::vector<Rect> freeShelfRects (freeShelves.size());
    for (machine i = 0; i < freeShelfRects.size(); i++)
    {
        freeShelfRects[i] = Rect {0, freeShelves[i].x, dims.x, ToLength(freeShelves[i])};
    }

    std::vector<Rect> freeSlots;
    for (const Shelf & shelf : shelves)
    {
        auto shelfFreeSlots = shelf.GetFreeSlots();
        for (auto freeSlot : shelfFreeSlots)
        {
            freeSlots.emplace_back(freeSlot.x, shelf.crossEndPoints.x, ToLength(freeSlot), shelf.minMaxSize.y);
        }
    }
    return std::make_pair(freeShelfRects, freeSlots);
}

std::vector<Rect> GlyphTextureStepped::GetModifiedFreeRects()
{
    std::vector<Rect> removedShelves;
    std::vector<Rect> removedSlots;

    auto freeShelfSlotSpace = GetFreeShelfSlotSpace();
    std::sort(freeShelfSlotSpace.first.begin(), freeShelfSlotSpace.first.end());
    std::sort(freeShelfSlotSpace.second.begin(), freeShelfSlotSpace.second.end());

    std::set_difference(prevFreeShelfSlotSpace.first.begin(), prevFreeShelfSlotSpace.first.end(),
                        freeShelfSlotSpace.first.begin(), freeShelfSlotSpace.first.end(),
                        std::inserter(removedShelves, removedShelves.begin()));

    std::set_difference(prevFreeShelfSlotSpace.second.begin(), prevFreeShelfSlotSpace.second.end(),
                        freeShelfSlotSpace.second.begin(), freeShelfSlotSpace.second.end(),
                        std::inserter(removedSlots, removedSlots.begin()));

    removedShelves.insert(removedShelves.end(), removedSlots.begin(), removedSlots.end());
    prevFreeShelfSlotSpace.first = freeShelfSlotSpace.first;
    prevFreeShelfSlotSpace.second = freeShelfSlotSpace.second;
    return removedShelves;
}