#include "GlyphTexture.h"

#include "EndPoints.h"
#include <algorithm>
#include <iterator>
#include <iostream>

void GlyphTexture::Render(FreeTypeWrapper &freeType)
{
    for (const auto& glyph : placedGlyphs)
    {
        auto bitmap = freeType.RenderGlyph(glyph.first);
        uint16_2 pos = {glyph.second.rect.x, glyph.second.rect.y};
        machine bitmapRowFlippedStart = bitmap.rows - 1;
        machine textureRowFlippedStart = dims.y - pos.y - 1;
        for (machine i = 0; i < bitmap.rows; i++)
        {
            machine bitmapRowFlipped = bitmapRowFlippedStart - i;
            auto rowStart = bitmap.buffer + bitmapRowFlipped * bitmap.pitch;
            auto dest = textureBuffer.data() + (textureRowFlippedStart - bitmapRowFlipped ) * dims.x + pos.x;

            memcpy(dest, rowStart, bitmap.pitch);
        }
    }
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

    std::set<GlyphKey> unused;
    std::set_difference(previouslyPlacedGlyphs.begin(), previouslyPlacedGlyphs.end(),
                        currentlyPlacedGlyphs.begin(), currentlyPlacedGlyphs.end(),
                        std::inserter(unused, unused.begin()));

    previouslyPlacedGlyphs = currentlyPlacedGlyphs;
    currentlyPlacedGlyphs.clear();

    for (const auto& key : unused)
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

    std::cout << "BAM!" << "\n";
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

bool GlyphTexture::Step(std::pair<GlyphKey, Glyph> toPlace)
{
    GlyphKey &glyphKey = toPlace.first;
    Glyph &glyph = toPlace.second;

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

bool GlyphTexture::GetGlyph(GlyphKey key, Glyph &glyph)
{
    if (auto search = placedGlyphs.find(key); search != placedGlyphs.end())
    {
        glyph = search->second;
        return true;
    }

    return false;
}

std::map<GlyphKey, Glyph>& GlyphTexture::GetGlyphs()
{
    return placedGlyphs;
}

std::pair<std::vector<Rect>, std::vector<Rect>> GlyphTexture::GetFreeShelfSlotSpace() const
{
    std::vector<Rect> freeShelfRects (freeShelves.size());
    for (machine i = 0; i < freeShelfRects.size(); i++)
    {
        auto height = static_cast<uint16>(freeShelves[i].y - freeShelves[i].x + 1);
        freeShelfRects[i] = Rect {0, freeShelves[i].x, dims.x, height};
    }

    std::vector<Rect> freeSlots;
    for (const Shelf & shelf : shelves)
    {
        auto shelfFreeSlots = shelf.GetFreeSlots();
        for (auto freeSlotX : shelfFreeSlots)
        {
            freeSlots.emplace_back(freeSlotX.x, shelf.crossEndPoints.x, freeSlotX.y - freeSlotX.x + 1, shelf.minMaxSize.y);
        }
    }
    return std::make_pair(freeShelfRects, freeSlots);
}