#include "GlyphAtlas.h"

GlyphAtlas::GlyphAtlas()
{
    rects = vector<Rect>();
}

void GlyphAtlas::Update(vector<Rect> newRects)
{
    sort(newRects.begin(), newRects.end(), CompareGreater);
    int totalArea = 0; // ideal packing
    for(Rect rect : newRects)
    {
        totalArea += rect.GetArea();
    }

    ushort dim = sqrt(totalArea * 2); // have double the ideal packing
    dims = {dim, dim};

    ushort usedWidth = 0, shelfHeight = 0, usedHeight = 0;

    for (int i = 0; i < newRects.size(); i++)
    {
        Rect rect = newRects[i];
        ushort potential = usedWidth + rect.w;
        if (potential > dims.x)
        {
            assert(usedWidth < dims.x);

            usedWidth = rect.w;
            rect.x = 0;
            usedHeight += shelfHeight;
            rect.y = usedHeight;
            shelfHeight = rect.h;

            newRects[i] = rect;
        }
        else
        {
            rect.x = usedWidth;
            rect.y = usedHeight;
            usedWidth += newRects[i].w;
            shelfHeight = max(newRects[i].h, shelfHeight);

            newRects[i] = rect;
        }
    }

    rects = newRects; // todo: obviously should not happen

    int z = 0;
}

bool GlyphAtlas::CompareGreater(const Rect &lhs, const Rect &rhs)
{
    return lhs.h > rhs.h;
}

