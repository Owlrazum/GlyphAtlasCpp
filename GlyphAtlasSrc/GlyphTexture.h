#pragma once

#include "PrimitiveTypes.h"
#include "Shelf.h"
#include "Glyph.h"
#include "GlyphKey.h"
#include "GlyphBitmap.h"

#include <vector>
#include <locale>
#include <set>
#include <map>

class GlyphTexture
{
public:
    GlyphTexture(std::vector<uint16> &shelfDelimitersArg,
                 std::vector<uint16> &widthDelimitersArg,
                 uint16_2 &textureDims,
                 uint8 idArg)
            : shelfDelimiters(shelfDelimitersArg),
              widthDelimiters(widthDelimitersArg),
              dims(textureDims),
              id(idArg)
    {
        freeShelves.emplace_back(0, dims.y - 1);
        textureBuffer = std::vector<uint8>(textureDims.x * textureDims.y);
    }

    void Render(std::map<GlyphKey, GlyphBitmap> &renderedBitmaps);
    void Update(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs);
    void RemoveUnused();
    unsigned char* GetRawBuffer() { return textureBuffer.data(); }
    bool IsEmpty() { return placedGlyphs.empty(); }

protected:
    uint16_2 dims;
    uint8 id;

    std::vector<unsigned char> textureBuffer;

    std::vector<Shelf> shelves;
    std::vector<uint16_2> freeShelves; // uint2_16.x - yMin pos, uint2_16.y - yMax pos

    std::map<GlyphKey, Glyph> placedGlyphs; // we need prev and cur, because placedGlyph is basically a mixture of both.

    std::set<GlyphKey> previouslyPlacedGlyphs;
    std::set<GlyphKey> currentlyPlacedGlyphs; // in other words, glyphs that were placed during this iteration.

    std::reference_wrapper<std::vector<uint16>> shelfDelimiters;
    std::reference_wrapper<std::vector<uint16>> widthDelimiters;

    [[nodiscard]] bool ContainsGlyph(const GlyphKey &key) const;
    bool FitInExistingSpot(std::pair<GlyphKey, Glyph> &glyph, uint16 slotWidth);

    bool CreateShelf(std::pair<GlyphKey, Glyph> &glyph, uint16 slotWidth);

    static void SplitFreeSpace(uint16_2 &freeShelf, uint16 splitHeight);
    void RemoveShelf(std::vector<Shelf>::iterator shelfToRemove);
    void ClaimFreeShelf(uint16_2 &freeShelf);
};