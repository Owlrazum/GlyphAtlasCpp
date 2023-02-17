#include "GlyphTexture.h"

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

    freeSpacesForShelves.emplace_back(0, 0, dims.x, dims.y);
    isInitialized = true;
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
        if (ContainsGlyph(glyphKey))
        {
            glyph.textureId = id;
            updateGlyphs.erase(updateGlyphs.begin() + i); // not using erase remove idiom to preserve the sort order
            i--;
            continue;
        }

        bool found = FitInExistingSpot(glyph);
        if (found)
        {
            placedGlyphs.insert(std::make_pair(glyphKey, glyph));
            updateGlyphs.erase(updateGlyphs.begin() + i); // not using erase remove idiom to preserve the sort order
            i--;
            continue;
        }

        bool shouldPlace = CreateShelf(glyph);
        if (shouldPlace)
        {
            placedGlyphs.insert(std::make_pair(glyphKey, glyph));
            updateGlyphs.erase(updateGlyphs.begin() + i); // not using erase remove idiom to preserve the sort order
            i--;
            continue;
        }
    }
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
    std::vector<Rect> freeShelves (freeSpacesForShelves);
    std::vector<Rect> freeSlots;
    for (const Shelf & shelf : shelves)
    {
        auto shelfFreeSlots = shelf.GetFreeSlots();
        freeSlots.insert(freeSlots.begin(), shelfFreeSlots.begin(), shelfFreeSlots.end());
    }
    return std::make_pair(freeShelves, freeSlots);
}

/// returns whether appropriate spot was found
bool GlyphTexture::FitInExistingSpot(Glyph& glyph)
{
    for (auto & shelf : shelves)
    {
        ushort h = glyph.rect.h;
        if (h >= shelf.minMaxHeight.x && h <= shelf.minMaxHeight.y) // let's assume that shelves are sorted by height, so the first match is correct
        {
            bool doesFit = shelf.TryAdd(glyph);
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
bool GlyphTexture::CreateShelf(Glyph& glyph)
{
    Pair shelfMinMaxHeight {0, 0};
    bool delimitersValid = false;
    for (int i = 1; i < shelfDelimiters.size(); i++)
    {
        if (glyph.rect.h <= shelfDelimiters[i])
        {
            shelfMinMaxHeight.x = shelfDelimiters[i - 1];
            shelfMinMaxHeight.y = shelfDelimiters[i];
            delimitersValid = true;
            break;
        }
    }

    if (!delimitersValid)
    {
        throw std::out_of_range("received delimiters for shelves are not big enough to contain the shelfRect: " + glyph.ToString());
    }

    for (int i = 0; i < freeSpacesForShelves.size(); i++)
    {
        if (freeSpacesForShelves[i].h >= shelfMinMaxHeight.y)
        {
            auto splitted = SplitFreeSpace(freeSpacesForShelves[i], shelfMinMaxHeight.y);
            freeSpacesForShelves.erase(freeSpacesForShelves.begin() + i);

            freeSpacesForShelves.push_back(splitted.second);

            Pair pos = {splitted.first.x, splitted.first.y};
            ushort width = {splitted.first.w};
            Shelf created {pos, width, shelfMinMaxHeight, widthDelimiters};
            bool doesFit = created.TryAdd(glyph);
            if (!doesFit)
            {
                throw std::out_of_range("The shelfRect width too large to fit into just created shelf");
            }

            shelves.push_back(created);
            return true;
        }
    }

    return false;
}

std::pair<Rect, Rect> GlyphTexture::SplitFreeSpace(Rect& freeSpace, ushort splitHeight)
{
    Rect first {freeSpace.x, freeSpace.y, freeSpace.w, splitHeight};
    Rect second {freeSpace.x, static_cast<ushort>(freeSpace.y + splitHeight),
                 freeSpace.w, static_cast<ushort>(freeSpace.h - splitHeight)};
    return std::make_pair(first, second);
}
