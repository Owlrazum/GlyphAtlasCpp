#include "GlyphTexture.h"

#include "Algorithms.h"
#include <algorithm>
#include <iterator>
#include <iostream>

void GlyphTexture::Render(std::map<GlyphKey, GlyphBitmap> &renderedBitmaps)
{
    for (const auto& glyph : placedGlyphs)
    {
        auto& bitmap = renderedBitmaps.find(glyph.first)->second;//freeType.RenderGlyph(glyph.first);
        uint16_2 pos = {glyph.second.rect.x, glyph.second.rect.y};
        machine bitmapRowFlippedStart = bitmap.dims.y - 1;
        machine textureRowFlippedStart = dims.y - pos.y - 1;
        for (machine i = 0; i < bitmap.dims.y; i++)
        {
            machine bitmapRowFlipped = bitmapRowFlippedStart - i;
            auto rowStart = bitmap.buffer + bitmapRowFlipped * bitmap.dims.x;
            auto dest = textureBuffer.data() + (textureRowFlippedStart - bitmapRowFlipped ) * dims.x + pos.x;

            memcpy(dest, rowStart, bitmap.dims.x);
        }
    }
}

bool GlyphTexture::ContainsGlyph(const GlyphKey &key) const
{
    return placedGlyphs.find(key) != placedGlyphs.end();
}

/// Erases glyphs that are placed from glyphs argument;
void GlyphTexture::Update(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs)
{
    for(machine i = 0; i < updateGlyphs.size(); i++)
    {
        auto updateGlyph = updateGlyphs[i];
        GlyphKey &glyphKey = updateGlyph.first;
        Glyph &glyph = updateGlyph.second;

        uint16 slotWidth = 0;
        for (uint16 widthDelimiter : widthDelimiters)
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
            updateGlyphs.erase(updateGlyphs.begin() + i); // not using erase remove idiom to preserve the sort order
            currentlyPlacedGlyphs.insert(glyphKey);
            i--;
            continue;
        }

        bool found = FitInExistingSpot(updateGlyph, slotWidth);
        if (found)
        {
            placedGlyphs.insert(updateGlyph);
            updateGlyphs.erase(updateGlyphs.begin() + i); // not using erase remove idiom to preserve the sort order
            currentlyPlacedGlyphs.insert(glyphKey);
            i--;
            continue;
        }

        bool wasPlaced = CreateShelf(updateGlyph, slotWidth);
        if (wasPlaced)
        {
            placedGlyphs.insert(updateGlyph);
            updateGlyphs.erase(updateGlyphs.begin() + i); // not using erase remove idiom to preserve the sort order
            currentlyPlacedGlyphs.insert(glyphKey);
            i--;
            continue;
        }
    }

    for (auto& shelf : shelves)
    {
        shelf.CheckIntegrity();
    }
}

/// returns whether appropriate spot was found
bool GlyphTexture::FitInExistingSpot(std::pair<GlyphKey, Glyph> &glyph, uint16 slotWidth)
{
    for (auto & shelf : shelves)
    {
        uint16 h = glyph.second.rect.h;
        if (h >= shelf.minMaxSize.x && h <= shelf.minMaxSize.y)
        {
            bool doesFit = shelf.TryAdd(glyph, slotWidth);
            if (doesFit)
            {
                shelf.CheckIntegrity();
                return true;
            }
        }
    }

    return false;
}

uint16_2 GetShelfMinMaxHeight(uint16 h, const std::vector<uint16>& delimiters)
{
    if (h <= delimiters[0])
    {
        return {0, delimiters[0]};
    }
    else
    {
        uint16_2 shelfMinMaxHeight {0, 0};
        bool delimitersValid = false;

        for (machine i = 1; i < delimiters.size(); i++)
        {
            if (h <= delimiters[i])
            {
                shelfMinMaxHeight.x = delimiters[i - 1];
                shelfMinMaxHeight.y = delimiters[i];
                delimitersValid = true;
                break;
            }
        }

        if (!delimitersValid)
        {
            throw std::out_of_range("received delimiters for shelves are not big enough to contain the shelfRect: " + std::to_string(h));
        }

        return shelfMinMaxHeight;
    }
}

/// creates and adds shelfRect to the shelf
// todod add several texture handling
bool GlyphTexture::CreateShelf(std::pair<GlyphKey, Glyph> &glyph, uint16 slotWidth)
{
    auto shelfMinMaxHeight = GetShelfMinMaxHeight(glyph.second.rect.h, shelfDelimiters);

    for (machine i = 0; i < freeShelves.size(); i++)
    {
        uint16 h = freeShelves[i].y - freeShelves[i].x + 1;
        if (shelfMinMaxHeight.y <= h)
        {
            uint16_2 mainEndPointsArg {0, static_cast<uint16>(dims.x - 1)};
            auto crossEnd = static_cast<uint16>(freeShelves[i].x + shelfMinMaxHeight.y - 1);
            uint16_2 crossEndPointsArg {freeShelves[i].x, crossEnd};
            Shelf created {mainEndPointsArg, crossEndPointsArg, shelfMinMaxHeight};

            created.TryAdd(glyph, slotWidth);

            shelves.push_back(created);

            SplitFreeSpace(freeShelves[i], shelfMinMaxHeight.y); // updates freeSpaceForShelf.x, so it shrinks in size
            if (freeShelves[i].y - freeShelves[i].x <= 0)
            {
                freeShelves.erase(freeShelves.begin() + i);
            }
            return true;
        }
    }

    return false;
}

void GlyphTexture::SplitFreeSpace(uint16_2 &freeShelf, uint16 splitHeight)
{
    machine occupiedY = freeShelf.x + splitHeight;
    freeShelf.x = occupiedY;
}

void GlyphTexture::RemoveUnused()
{
    if (previouslyPlacedGlyphs.empty())
    {
        previouslyPlacedGlyphs = currentlyPlacedGlyphs;
        currentlyPlacedGlyphs.clear();
        return;
    }

    std::vector<GlyphKey> unusedKeys;

    std::set_difference(previouslyPlacedGlyphs.begin(), previouslyPlacedGlyphs.end(),
                        currentlyPlacedGlyphs.begin(), currentlyPlacedGlyphs.end(),
                        std::inserter(unusedKeys, unusedKeys.begin()));

    previouslyPlacedGlyphs = currentlyPlacedGlyphs;
    currentlyPlacedGlyphs.clear();

    for (const auto& key : unusedKeys)
    {
        for (machine i = 0; i < shelves.size(); i++)
        {
            auto keyRemovedShelfEmpty = shelves[i].TryRemove(key);
            if (keyRemovedShelfEmpty.first)
            {
                if (keyRemovedShelfEmpty.second)
                {
                    RemoveShelf(shelves.begin() + i);
                }
                break;
            }
        }
        placedGlyphs.erase(key);
    }
}

void GlyphTexture::RemoveShelf(std::vector<Shelf>::iterator shelfToRemove)
{
    ClaimFreeShelf(shelfToRemove->crossEndPoints);
    shelves.erase(shelfToRemove);
}

void GlyphTexture::ClaimFreeShelf(uint16_2 &freeShelf)
{
    bool isMerged = MergeIntoIfPossible(freeShelf, freeShelves);
    if (!isMerged)
    {
        freeShelves.push_back(freeShelf);
    }
}