#include "FileUtils.h"
#include "GlyphAtlas.h"
#include "CppExport.h"

#include <vector>

uint2_16 maxTextureDims = {512, 512};
GlyphAtlas glyphAtlas = GlyphAtlas(maxTextureDims);

// the containers are flushed after each step.
std::vector<std::vector<std::pair<GlyphKey, Glyph>>> passKeysByTestNumber {};
std::vector<std::vector<Glyph>> placedGlyphsByTexture {};
std::vector<std::vector<Rect>> freeShelvesByTexture {};
std::vector<std::vector<Rect>> freeSlotsByTexture {};

struct int3
{
    machine x;
    machine y;
    machine z;
};

extern "C"
{
    DLLEXPORT machine InitTest(machine testNumber)
    {
        passKeysByTestNumber = ReadGlyphKeysByLine(GetTestGlyphKeysPath(testNumber));
        return static_cast<int>(passKeysByTestNumber.size());
    }

    DLLEXPORT machine InitPass(machine passNumber)
    {
        auto keys = passKeysByTestNumber[passNumber];
        glyphAtlas.InitPass(keys);
        return static_cast<int>(keys.size());
    }

    // returns textureCount;
    DLLEXPORT machine Step()
    {
        machine texturesCount = glyphAtlas.Step();

        placedGlyphsByTexture.clear();
        for (machine i = 0; i < texturesCount; i++)
        {
            auto textureGlyphuint2_16s = glyphAtlas.GetGlyphsFromTexture(i);
            std::vector<Glyph> textureGlyphs;
            for (auto pair : textureGlyphuint2_16s)
            {
                textureGlyphs.push_back(pair.second);
            }
            placedGlyphsByTexture.push_back(textureGlyphs);
        }

        freeShelvesByTexture.clear();
        freeSlotsByTexture.clear();
        for (machine i = 0; i < texturesCount; i++)
        {
            auto freeRects = glyphAtlas.GetFreeShelfSlotSpace(i);
            freeShelvesByTexture.push_back(freeRects.first);
            freeSlotsByTexture.push_back(freeRects.second);
        }

        return texturesCount;
    }

    // x - placed, y - freeShelves, z - freeSlots
    DLLEXPORT int3 GetRectsCount(machine textureId)
    {
        return {
            static_cast<int>(placedGlyphsByTexture[textureId].size()),
            static_cast<int>(freeShelvesByTexture[textureId].size()),
            static_cast<int>(freeSlotsByTexture[textureId].size())
        };
    }

    DLLEXPORT CRect GetPlacedGlyph(machine textureId, machine glyphIndex)
    {
        return placedGlyphsByTexture[textureId][glyphIndex].rect;
    }
    DLLEXPORT CRect GetFreeShelfRect(machine textureId, machine freeShelfIndex)
    {
        return freeShelvesByTexture[textureId][freeShelfIndex];
    }
    DLLEXPORT CRect GetFreeSlotRect(machine textureId, machine freeSlotIndex)
    {
        return freeSlotsByTexture[textureId][freeSlotIndex];
    }
}