#include "FileUtils.h"
#include "GlyphAtlas.h"
#include "CppExport.h"

#include <vector>

GlyphAtlas glyphAtlas = GlyphAtlas({16, 32, 64}, {8, 16, 24, 32, 40, 48, 56, 64}, {512, 512});

// the containers are flushed after each step.
std::vector<std::vector<GlyphKey>> passKeysByTestNumber {};
std::vector<std::vector<Glyph>> placedGlyphsByTexture {};
std::vector<std::vector<Rect>> freeShelvesByTexture {};
std::vector<std::vector<Rect>> freeSlotsByTexture {};

struct int3
{
    int x;
    int y;
    int z;
};

extern "C"
{
    DLLEXPORT int InitTest(int testNumber)
    {
        passKeysByTestNumber = ReadGlyphKeysByLine(GetDataPath(testNumber));
        return static_cast<int>(passKeysByTestNumber.size());
    }

    DLLEXPORT int InitPass(int passNumber)
    {
        return glyphAtlas.InitPass(passKeysByTestNumber[passNumber]);
    }

    // returns textureCount;
    DLLEXPORT int Step()
    {
        int texturesCount = glyphAtlas.Step();

        placedGlyphsByTexture.clear();
        for (int i = 0; i < texturesCount; i++)
        {
            auto textureGlyphPairs = glyphAtlas.GetGlyphsFromTexture(i);
            std::vector<Glyph> textureGlyphs;
            for (auto pair : textureGlyphPairs)
            {
                textureGlyphs.push_back(pair.second);
            }
            placedGlyphsByTexture.push_back(textureGlyphs);
        }

        freeShelvesByTexture.clear();
        freeSlotsByTexture.clear();
        for (int i = 0; i < texturesCount; i++)
        {
            auto freeRects = glyphAtlas.GetFreeShelfSlotSpace(i);
            freeShelvesByTexture.push_back(freeRects.first);
            freeSlotsByTexture.push_back(freeRects.second);
        }

        return texturesCount;
    }

    // x - placed, y - freeShelves, z - freeSlots
    DLLEXPORT int3 GetRectsCount(int textureId)
    {
        return {
            static_cast<int>(placedGlyphsByTexture[textureId].size()),
            static_cast<int>(freeShelvesByTexture[textureId].size()),
            static_cast<int>(freeShelvesByTexture[textureId].size())
        };
    }

    DLLEXPORT Rect GetPlacedGlyph(int textureId, int glyphIndex)
    {
        return placedGlyphsByTexture[textureId][glyphIndex].rect;
    }
    DLLEXPORT Rect GetFreeShelfRect(int textureId, int freeShelfIndex)
    {
        return freeShelvesByTexture[textureId][freeShelfIndex];
    }
    DLLEXPORT Rect GetFreeSlotRect(int textureId, int freeSlotIndex)
    {
        return freeSlotsByTexture[textureId][freeSlotIndex];
    }
}