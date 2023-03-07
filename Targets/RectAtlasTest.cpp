#include "FileUtils.h"
#include "GlyphAtlas.h"
#include "CppExport.h"

#include <vector>

uint16_2 maxTextureDims = {512, 512};
GlyphAtlas glyphAtlas = GlyphAtlas(maxTextureDims);

// the containers are flushed after each step.
std::vector<std::vector<std::pair<GlyphKey, Glyph>>> passKeysByTestNumber {};
std::vector<std::vector<std::pair<GlyphKey, Glyph>>> placedGlyphsByTexture {};
std::vector<std::vector<Rect>> freeShelvesByTexture {};
std::vector<std::vector<Rect>> freeSlotsByTexture {};

struct int3
{
    uint32 x;
    uint32 y;
    uint32 z;
};

extern "C"
{
    DLLEXPORT machine InitTest(machine testNumber)
    {
        passKeysByTestNumber = ReadGlyphKeysByLine(GetTestGlyphKeysPath(testNumber));
        return static_cast<int>(passKeysByTestNumber.size());
    }

    // returns stepsCount
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
            auto textureGlyphsMap = glyphAtlas.GetGlyphsFromTexture(i);
            std::vector<std::pair<GlyphKey, Glyph>> textureGlyphs(textureGlyphsMap.size());
            for (auto pair : textureGlyphsMap)
            {
                textureGlyphs.emplace_back(pair);
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

    DLLEXPORT void RemoveUnused()
    {
        glyphAtlas.RemoveUnused();
    }

    // x - placed, y - freeShelves, z - freeSlots
    DLLEXPORT int3 GetRectsCount(machine textureId)
    {
        return {
            static_cast<uint32>(placedGlyphsByTexture[textureId].size()),
            static_cast<uint32>(freeShelvesByTexture[textureId].size()),
            static_cast<uint32>(freeSlotsByTexture[textureId].size())
        };
    }

    DLLEXPORT CRect GetPlacedGlyph(machine textureId, machine glyphIndex)
    {
        return placedGlyphsByTexture[textureId][glyphIndex].second.rect;
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

int main()
{
    auto passCount = InitTest(0);
    for (int p = 0; p < passCount; p++)
    {
        auto stepsCount = InitPass(p);
        for (machine s = 0; s < stepsCount; s++)
        {
            auto texturesCount = Step();
            for (machine t = 0; t < texturesCount; t++)
            {
                int3 rectsCounts = GetRectsCount(t);
                for (machine r = 0; r < rectsCounts.x; r++)
                {
                    GetPlacedGlyph(t, r);
                }
                for (machine r = 0; r < rectsCounts.y; r++)
                {
                    GetFreeShelfRect(t, r);
                }
                for (machine r = 0; r < rectsCounts.z; r++)
                {
                    GetFreeSlotRect(t, r);
                }
            }
        }
        RemoveUnused();
    }
    passKeysByTestNumber = ReadGlyphKeysByLine(GetTestGlyphKeysPath(0));
}