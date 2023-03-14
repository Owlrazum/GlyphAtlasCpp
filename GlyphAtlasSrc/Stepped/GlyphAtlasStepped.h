#pragma once

#include "GlyphAtlas.h"
#include "GlyphTextureStepped.h"
#include <queue>

class GlyphAtlasStepped : public GlyphAtlas
{
public:
    explicit GlyphAtlasStepped(uint16_2 textureMaxDimsArg)
            : GlyphAtlas(textureMaxDimsArg)
    {
        stepIndex = 0;
    };

    void InitPass(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs);

    void UpdateStep(); // returns number of used textures

    uint32 InitRemovePlacesPass();
    CRect RemovePlacedStep();

    uint32 InitGetModifiedFreePass();
    CRect GetModifiedFreeStep();

    [[nodiscard]] std::pair<std::vector<Rect>, std::vector<Rect>>
    GetFreeShelfSlotSpace(machine textureId) const // the first vector is freeSpace for the shelves
    {
        return steppedTextures[textureId].GetFreeShelfSlotSpace();
    }

    std::map<GlyphKey, Glyph> GetGlyphsFromTexture(uint8 textureId)
    {
        return steppedTextures[textureId].GetGlyphs();
    }

    uint32 GetSteppedTexturesCount()
    {
        return steppedTextures.size();
    }

private:
    machine stepIndex;
    std::vector<GlyphTextureStepped> steppedTextures;
    std::vector<std::pair<GlyphKey, Glyph>> queue;

    std::vector<std::vector<GlyphKey>> removePlacedQueue;
    std::vector<Rect> removeFreeQueue;
};