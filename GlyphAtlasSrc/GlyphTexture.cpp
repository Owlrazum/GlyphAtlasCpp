#include "GlyphTexture.h"

#include "EndPoints.h"
#include <algorithm>
#include <iterator>
//#include <cmath>

void GlyphTexture::Initialize()//std::vector<Rect> &newRects, int idealAreaMultiplier)
{
//    int idealPackingArea = 0;
//    for(Rect shelfRect : newRects)
//    {
//        idealPackingArea += shelfRect.GetArea();
//    }
//
//    auto dim = static_cast<ushort>(sqrt(idealPackingArea * idealAreaMultiplier));
//    dims = {std::min(dim, dims.x), std::min(dim, dims.y)};

    freeShelves.emplace_back(0, dims.y - 1);
    isInitialized = true;
}

std::vector<std::pair<GlyphKey, Glyph>> GlyphTexture::GetGlyphs() const
{
    std::vector<std::pair<GlyphKey, Glyph>> toReturn;
    for (const auto& placedGlyph : placedGlyphs)
    {
        toReturn.emplace_back(placedGlyph);
    }
    return toReturn;
}


std::pair<std::vector<Rect>, std::vector<Rect>> GlyphTexture::GetFreeShelfSlotSpace() const
{
    std::vector<Rect> freeShelfRects (freeShelves.size());
    for (int i = 0; i < freeShelfRects.size(); i++)
    {
        auto height = static_cast<ushort>(freeShelves[i].y + 1);
        freeShelfRects[i] = Rect {0, freeShelves[i].x, dims.x, height};
    }

    std::vector<Rect> freeSlots;
    for (const Shelf & shelf : shelves)
    {
        auto shelfFreeSlots = shelf.GetFreeSlots();
        for (auto freeSlotX : shelfFreeSlots)
        {
            freeSlots.emplace_back(freeSlotX.x, shelf.crossEndPoints.x, freeSlotX.y - freeSlotX.x - 1, shelf.minMaxSize.y);
        }
    }
    return std::make_pair(freeShelfRects, freeSlots);
}

bool GlyphTexture::ContainsGlyph(const GlyphKey &glyphKey) const
{
    if (auto font = placedGlyphs.find(glyphKey); font != placedGlyphs.end())
    {
        return true;
    }
    return false;
}

/// Erases glyphs that are placed from glyphs argument;
void GlyphTexture::Update(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs)
{
    if (!isInitialized)
    {
        Initialize();
    }

    for(int i = 0; i < updateGlyphs.size(); i++)
    {
        auto updateGlyph = updateGlyphs[i];
        GlyphKey &glyphKey = updateGlyph.first;
        Glyph &glyph = updateGlyph.second;

        ushort slotWidth = 0;
        for (ushort widthDelimiter : widthDelimiters)
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
            placedGlyphs.insert(std::make_pair(glyphKey, glyph));
            updateGlyphs.erase(updateGlyphs.begin() + i); // not using erase remove idiom to preserve the sort order
            currentlyPlacedGlyphs.insert(glyphKey);
            i--;
            continue;
        }

        bool shouldPlace = CreateShelf(updateGlyph, slotWidth);
        if (shouldPlace)
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
bool GlyphTexture::FitInExistingSpot(std::pair<GlyphKey, Glyph> &glyph, ushort slotWidth)
{
    for (auto & shelf : shelves)
    {
        ushort h = glyph.second.rect.h;
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

/// creates and adds shelfRect to the shelf
// todod add several texture handling
bool GlyphTexture::CreateShelf(std::pair<GlyphKey, Glyph> &glyph, ushort slotWidth)
{
    Pair shelfMinMaxHeight {0, 0};
    bool delimitersValid = false;
    for (int i = 1; i < shelfDelimiters.size(); i++)
    {
        if (glyph.second.rect.h <= shelfDelimiters[i])
        {
            shelfMinMaxHeight.x = shelfDelimiters[i - 1];
            shelfMinMaxHeight.y = shelfDelimiters[i];
            delimitersValid = true;
            break;
        }
    }

    if (!delimitersValid)
    {
        throw std::out_of_range("received delimiters for shelves are not big enough to contain the shelfRect: " + glyph.second.ToString());
    }

    for (int i = 0; i < freeShelves.size(); i++)
    {
        ushort h = freeShelves[i].y - freeShelves[i].x + 1;
        if (shelfMinMaxHeight.y <= h)
        {
            Pair mainEndPointsArg {0, static_cast<ushort>(dims.x - 1)};
            auto crossEnd = static_cast<ushort>(freeShelves[i].x + shelfMinMaxHeight.y);
            Pair crossEndPointsArg {freeShelves[i].x, crossEnd};
            Shelf created {mainEndPointsArg, crossEndPointsArg, shelfMinMaxHeight};

            created.TryAdd(glyph, slotWidth);

            shelves.push_back(created);

            SplitFreeSpace(freeShelves[i], shelfMinMaxHeight.y); // updates freeSpaceForShelf.x, so it shrinks in size
            if (freeShelves[i].y - freeShelves[i].x == 0)
            {
                freeShelves.erase(freeShelves.begin() + i);
            }
            return true;
        }
    }

    return false;
}

void GlyphTexture::SplitFreeSpace(Pair &freeShelf, ushort splitHeight)
{
    int occupiedY = freeShelf.x + splitHeight + 1;
    assert(occupiedY >= 0); // if occupiedY equal to freeShelf.x, it is empty and should be deleted
    freeShelf.x = occupiedY + 1;
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
        for (int i = 0; i < shelves.size(); i++)
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

void GlyphTexture::ClaimFreeShelf(Pair &freeShelf)
{
    bool isMerged = MergeIntoIfPossible(freeShelf, freeShelves);
    if (!isMerged)
    {
        freeShelves.push_back(freeShelf);
    }
}
