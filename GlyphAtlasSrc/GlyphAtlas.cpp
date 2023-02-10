#include "GlyphAtlas.h"

void GlyphAtlas::InitializeDims(std::vector<Rect> newRects)
{
    int totalArea = 0; // ideal packing
    for(Rect rect : newRects)
    {
        totalArea += rect.GetArea();
    }

    ushort dim = sqrt(totalArea * 2); // have double the ideal packing
    dims = {dim, dim};

    freeSpacesForShelves.push_back({0, 0, dim, dim});

    isDimsInitialized = true;
}

void GlyphAtlas::Update(std::vector<Rect> newRects)
{
    sort(newRects.begin(), newRects.end(), CompareGreater);

    if (!isDimsInitialized)
    {
        InitializeDims(newRects);
    }

    for(int i = 0; i < newRects.size(); i++)
    {
        Rect& toFit = newRects[i];
        bool found = FitInExistingSpot(toFit);
        if (!found)
        {
            CreateShelf(toFit);
        }
        rects.push_back(toFit);
    }
}

bool GlyphAtlas::CompareGreater(const Rect &lhs, const Rect &rhs)
{
    return lhs.h > rhs.h;
}

/// returns whether appropiate spot was found
bool GlyphAtlas::FitInExistingSpot(Rect& rect)
{
    for (int i = 0; i < shelves.size(); i++)
    {
        if (rect.h <= shelves[i].rect.h) // let's assume that shelves are sorted by height, so the first match is correct
        {
            bool doesFit = shelves[i].TryAdd(rect);
            if (doesFit)
            {
                return true;
            }
        }
    }

    return false;
}

/// creates and adds rect to the shelf
// todo add several texture handling
void GlyphAtlas::CreateShelf(Rect& toFit)
{
    ushort shelfHeight = -1;
    for (int i = 0; i < shelfDelimiters.size(); i++)
    {
        ushort delimiter = shelfDelimiters[i];
        if (toFit.h <= delimiter)
        {
            shelfHeight = delimiter;
            break;
        }
    }

    if (shelfHeight < 0)
    {
        throw std::out_of_range("received delimiters for shelves are not big enough to contain the rect: " + toFit.ToString());
    }

    Pair pos {0, 0};
    for (int i = 0; i < freeSpacesForShelves.size(); i++)
    {
        if (shelfHeight <= freeSpacesForShelves[i].h)
        {
            auto splitted = SplitFreeSpace(freeSpacesForShelves[i], shelfHeight);
            freeSpacesForShelves.erase(freeSpacesForShelves.begin() + i);

            freeSpacesForShelves.push_back(splitted.second);

            Shelf created {splitted.first, widthDelimiters};
            bool doesFit = created.TryAdd(toFit);
            if (!doesFit)
            {
                throw std::out_of_range("The rect width too large to fit into just created shelf");
            }

            shelves.push_back(created);
            return;
        }
    }

    throw std::out_of_range("no free space with sufficient dims for the shelf was found");
}

std::pair<Rect, Rect> GlyphAtlas::SplitFreeSpace(Rect& freeSpace, ushort splitHeight)
{
    Rect first {freeSpace.x, freeSpace.y, freeSpace.w, splitHeight};
    Rect second {freeSpace.x, static_cast<ushort>(freeSpace.y + splitHeight),
                 freeSpace.w, static_cast<ushort>(freeSpace.h - splitHeight)};
    return std::make_pair(first, second);
}

/* Version with one time packing
 * ushort usedWidth = 0, shelfHeight = 0, usedHeight = 0;
 *
 *    for (int i = 0; i < newRects.size(); i++)
 *    {
 *        Rect rect = newRects[i];
 *        ushort potential = usedWidth + rect.w;
 *        if (potential > dims.x)
 *        {
 *            assert(usedWidth < dims.x);
 *
 *            usedWidth = rect.w;
 *            rect.x = 0;
 *            usedHeight += shelfHeight;
 *            rect.y = usedHeight;
 *            shelfHeight = rect.h;
 *
 *            newRects[i] = rect;
 *        }
 *        else
 *        {
 *            rect.x = usedWidth;
 *            rect.y = usedHeight;
 *            usedWidth += newRects[i].w;
 *            shelfHeight = std::max(newRects[i].h, shelfHeight);
 *
 *            newRects[i] = rect;
 *        }
 *    }
 *
 *    rects = newRects;
 */