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
    GlyphTexture(const std::vector<uint16> &shelfDelimitersArg,
                 const std::vector<uint16> &widthDelimitersArg,
                 const uint16_2 &textureDims,
                 uint16 idArg)
            : shelfDelimiters(shelfDelimitersArg),
              widthDelimiters(widthDelimitersArg),
              dims(textureDims),
              id(idArg)//,
    {
        freeShelves.emplace_back(0, dims.y - 1);
        textureBuffer = std::vector<uint8>(textureDims.x * textureDims.y);
    }

    void Render(std::map<GlyphKey, GlyphBitmap> &renderedBitmaps);
    void Update(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs);
    void RemoveUnused();

    unsigned char* GetRawBuffer()
    {
        return textureBuffer.data();
    }

    [[nodiscard]] bool ContainsGlyph(const GlyphKey &key) const
    {
        return placedGlyphs.find(key) != placedGlyphs.end();
    }

    void MarkGlyph(const GlyphKey &key)
    {
        currentlyPlacedGlyphs.insert(key);
    }

    bool GetGlyph(GlyphKey key, Glyph& glyph); // returns whether the glyph was found

    machine GetGlyphCount()
    {
        return static_cast<int>(placedGlyphs.size());
    }

    [[nodiscard]] std::map<GlyphKey, Glyph>& GetGlyphs();
    [[nodiscard]] std::pair<std::vector<Rect>, std::vector<Rect>> GetFreeShelfSlotSpace() const; // the first vector is freeSpace for the shelves

    [[nodiscard]] uint16 GetId() const
    { return id; }

    bool Step(std::pair<GlyphKey, Glyph> toPlace);
private:
    uint16_2 dims;
    uint16 id;

    std::vector<unsigned char> textureBuffer;

    std::vector<Shelf> shelves;
    std::vector<uint16_2> freeShelves; // uint2_16.x - yMin pos, uint2_16.y - yMax pos

    std::map<GlyphKey, Glyph> placedGlyphs; // we need prev and cur, because placedGlyph is basically a mixture of both.
    std::set<GlyphKey> previouslyPlacedGlyphs;
    std::set<GlyphKey> currentlyPlacedGlyphs; // in other words, glyphs that were placed during this iteration.

    const std::vector<uint16> shelfDelimiters;
    const std::vector<uint16> widthDelimiters;

    bool FitInExistingSpot(std::pair<GlyphKey, Glyph> &glyph, uint16 slotWidth);

    bool CreateShelf(std::pair<GlyphKey, Glyph> &glyph, uint16 slotWidth);

    static void SplitFreeSpace(uint16_2 &freeShelf, uint16 splitHeight);
    void RemoveShelf(std::vector<Shelf>::iterator shelfToRemove);
    void ClaimFreeShelf(uint16_2 &freeShelf);
};