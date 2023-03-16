#pragma once

#include "GlyphTexture.h"

class GlyphTextureStepped : public GlyphTexture
{
public:
    GlyphTextureStepped(std::vector<uint16> &shelfDelimitersArg,
                 std::vector<uint16> &widthDelimitersArg,
                 uint16_2 &textureDims,
                 uint16_2 &unusedThresholds,
                 uint8 idArg)
            : GlyphTexture(shelfDelimitersArg, widthDelimitersArg, textureDims, unusedThresholds, idArg)
    {
    }

    [[nodiscard]] std::map<GlyphKey, Glyph>& GetGlyphs();
    [[nodiscard]] std::pair<std::vector<Rect>, std::vector<Rect>> GetFreeShelfSlotSpace() const; // the first vector is freeSpace for the shelves

    bool UpdateStep(std::pair<GlyphKey, Glyph> toPlace);
    std::vector<GlyphKey> CollectUnused();

//    void RecordFreeShelfSlotSpace();
    CRect RemoveStep(GlyphKey &keyToRemove);
    [[nodiscard]] std::vector<Rect> GetModifiedFreeRects();

private:
    std::pair<std::vector<Rect>, std::vector<Rect>> prevFreeShelfSlotSpace;
};