//
// Created by Abai on 07.02.2023.
//

/*
 * struct FontKey {
    uint8 fontId;
    float fontSize;
    float sdfPadding;
    TypeMask mask;
}
enum TypeMask {
    Sdf,
    Msdf,
    Raster,
    RasterOutline,
}

 GlyphHandle GetGlyph(FontKey key, GlyphId id);

 struct GlyphHandle { int textureId; ushort x, y, w, h; }
 */

#ifndef CLION_GLYPHATLAS_H
#define CLION_GLYPHATLAS_H

#include "Rect.h"
#include "Shelf.h"
#include <vector>
#include <cmath>

class GlyphAtlas
{
public:
    GlyphAtlas(std::vector<ushort> shelfDelimitersArg, std::vector<ushort> widthDelimitersArg)
    : shelfDelimiters(shelfDelimitersArg), widthDelimiters(widthDelimitersArg), dims({0, 0})
    {
        rects = std::vector<Rect>();
        shelves = std::vector<Shelf>();
        freeSpacesForShelves = std::vector<Rect>();
    };

    void Update(std::vector<Rect> newRects);

    std::vector<Rect> GetRects() const
    {
        return rects;
    }

    Pair GetDims() const
    { return dims; }

private:
    bool isDimsInitialized;
    void InitializeDims(std::vector<Rect> newRects);

    std::vector<Rect> rects;
    std::vector<Shelf> shelves;
    std::vector<Rect> freeSpacesForShelves;
    Pair dims;

    const std::vector<ushort> shelfDelimiters;
    const std::vector<ushort> widthDelimiters;

    bool FitInExistingSpot(Rect& rect);
    void CreateShelf(Rect& rect);
    std::pair<Rect, Rect> SplitFreeSpace(Rect& freeSpace, ushort splitHeight);

    static bool CompareGreater(const Rect &lhs, const Rect &rhs);
};

#endif //CLION_GLYPHATLAS_H
