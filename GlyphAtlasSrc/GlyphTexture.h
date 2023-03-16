#pragma once

#include "PrimitiveTypes.h"
#include "Shelf.h"
#include "Glyph.h"
#include "GlyphKey.h"
#include "GlyphBitmap.h"

#include <vector>
#include <set>
#include <map>

class GlyphTexture
{
public:
    GlyphTexture(std::vector<uint16> &shelfDelimitersArg,
                 std::vector<uint16> &widthDelimitersArg,
                 uint16_2 &textureDims,
                 uint16_2 unusedThresholdsArg,
                 uint8 idArg)
            : shelfDelimiters(shelfDelimitersArg),
              widthDelimiters(widthDelimitersArg),
              dims(textureDims),
              unusedThresholds(unusedThresholdsArg),
              id(idArg)
    {
        freeShelves.emplace_back(0, dims.y - 1);
        placedGlyphsCount = 0;
        textureBuffer = std::vector<uint8>(textureDims.x * textureDims.y);
    }

    void Update(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs,
                std::map<GlyphKey, Glyph> &placedGlyphs);
    void Render(std::map<GlyphKey, GlyphBitmap> &renderedBitmaps, std::map<GlyphKey, Glyph> &placedGlyphs,
                bool hasSinglePixelPadding);
    void RemoveGlyph(const GlyphKey &glyphKey, std::map<GlyphKey, Glyph> &placedGlyphs, bool hasSinglePixelPadding);

    unsigned char* GetRawBuffer() { return textureBuffer.data(); }
    [[nodiscard]] bool IsEmpty() const { return placedGlyphsCount == 0; };

protected:
    uint16_2 dims;
    uint8 id;

    uint16_2 unusedThresholds;

    uint16 placedGlyphsCount;

    std::vector<uint8> textureBuffer;

    std::vector<Shelf> shelves;
    std::vector<uint16_2> freeShelves; // uint2_16.x -> yMin pos, uint2_16.y -> yMax pos

    std::reference_wrapper<std::vector<uint16>> shelfDelimiters;
    std::reference_wrapper<std::vector<uint16>> widthDelimiters;

    bool FitInExistingSpot(std::pair<GlyphKey, Glyph> &glyph, uint16 slotWidth);
    bool CreateShelf(std::pair<GlyphKey, Glyph> &glyph, uint16 slotWidth);
    static void SplitFreeSpace(uint16_2 &freeShelf, uint16 splitHeight);

    void ClearRectOfTextureBuffer(CRect rect, bool hasSinglePixelPadding);

    void RemoveShelf(std::vector<Shelf>::iterator shelfToRemove);
    void ClaimFreeShelf(uint16_2 &freeShelf);

    [[nodiscard]] uint16 GetSlotWidth(const Glyph &glyph) const;
};