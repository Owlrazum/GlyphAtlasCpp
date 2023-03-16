#include "GlyphTexture.h"

#include "Algorithms.h"
#include <algorithm>
#include <iterator>
#include <iostream>

void GlyphTexture::Render(std::map<GlyphKey, GlyphBitmap> &renderedBitmaps, std::map<GlyphKey, Glyph> &placedGlyphs,
                          bool hasSinglePixelPadding)
{
    for (const auto& glyph : placedGlyphs)
    {
        if (glyph.second.textureId != id)
        {
            continue;
        }

        auto& bitmap = renderedBitmaps.find(glyph.first)->second;//freeType.RenderGlyph(glyph.first);
        uint16_2 pos = {glyph.second.rect.x, glyph.second.rect.y};
        if (hasSinglePixelPadding)
        {
            pos.x++;
            pos.y++;
        }

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

void GlyphTexture::ClearRectOfTextureBuffer(CRect rect, bool hasSinglePixelPadding)
{
    auto posY = dims.y - rect.y - 1;
    auto posX = rect.x;
    auto width = rect.w;
    auto height = rect.h;
    if (hasSinglePixelPadding)
    {
        posX++;
        posY++;
        width -= 2;
        height -= 2;
    }

    auto dest = textureBuffer.data() + posY * dims.x + posX;
    for (int i = 0; i < height; i++)
    {
        memset(dest, 0, width);
        dest -= dims.x;
    }
}

uint16 GlyphTexture::GetSlotWidth(const Glyph &glyph) const
{
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
    return slotWidth;
}

/// Erases glyphs that are placed from glyphs argument;
void GlyphTexture::Update(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs,
                          std::map<GlyphKey, Glyph> &placedGlyphs)
{
    for(machine i = 0; i < updateGlyphs.size(); i++)
    {
        auto& updateGlyph = updateGlyphs[i];
        GlyphKey &glyphKey = updateGlyphs[i].first;
        Glyph &glyph = updateGlyphs[i].second;

        uint16 slotWidth = GetSlotWidth(glyph);

        bool found = FitInExistingSpot(updateGlyph, slotWidth);
        if (found)
        {
            glyph.textureId = id;
            placedGlyphs.insert(updateGlyph);
            updateGlyphs.erase(updateGlyphs.begin() + i); // not using erase remove idiom to preserve the sort order
            placedGlyphsCount++;
            i--;
            continue;
        }

        bool wasPlaced = CreateShelf(updateGlyph, slotWidth);
        if (wasPlaced)
        {
            glyph.textureId = id;
            placedGlyphs.insert(updateGlyph);
            updateGlyphs.erase(updateGlyphs.begin() + i); // not using erase remove idiom to preserve the sort order
            placedGlyphsCount++;
            i--;
            continue;
        }
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

void GlyphTexture::RemoveGlyph(const GlyphKey &glyphKey, std::map<GlyphKey, Glyph> &placedGlyphs, bool hasSinglePixelPadding)
{
    for (machine i = 0; i < shelves.size(); i++)
    {
        auto keyRemovedShelfEmpty = shelves[i].TryRemove(glyphKey);
        if (keyRemovedShelfEmpty.first)
        {
            if (keyRemovedShelfEmpty.second)
            {
                RemoveShelf(shelves.begin() + i);
            }
            break;
        }
    }
    ClearRectOfTextureBuffer(placedGlyphs.find(glyphKey)->second.rect, hasSinglePixelPadding);
    placedGlyphs.erase(glyphKey);
    placedGlyphsCount--;
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