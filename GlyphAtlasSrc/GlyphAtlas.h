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

#pragma once

#include "Rect.h"
#include "Shelf.h"
#include "Glyph.h"

#include <vector>

class GlyphAtlas
{
public:
    GlyphAtlas(std::vector<ushort> shelfDelimitersArg, std::vector<ushort> widthDelimitersArg)
            : shelfDelimiters(shelfDelimitersArg), widthDelimiters(widthDelimitersArg)
    {
        glyphs = std::vector<Glyph>();
        shelves = std::vector<Shelf>();
        freeSpacesForShelves = std::vector<Rect>();

        textures = std::vector<Pair>();
    };

    void Update(std::vector<Rect> newRects);

    std::vector<Glyph> GetGlyphs() const
    {
        return glyphs;
    }

    Pair GetTextureDims(int textureId) const
    { return textures[textureId]; }

    int GetTextureCount()
    {
        return textures.size();
    }

private:
    bool isDimsInitialized;
    int glyphIdCount;

    void InitializeDims(std::vector<Rect> newRects);

    std::vector<Glyph> glyphs;
    std::vector<Shelf> shelves;
    std::vector<Rect> freeSpacesForShelves;
    std::vector<Pair> textures;

    const std::vector<ushort> shelfDelimiters;
    const std::vector<ushort> widthDelimiters;

    bool FitInExistingSpot(Rect &rect);

    void CreateShelf(Rect &rect);

    std::pair<Rect, Rect> SplitFreeSpace(Rect &freeSpace, ushort splitHeight);

    static bool CompareGreater(const Rect &lhs, const Rect &rhs);
};