// #include "FileUtils.h"
// #include "GlyphAtlasStepped.h"
// #include "CppExport.h"

// #include <vector>

// struct int32_2
// {
//    int32 x;
//    int32 y;
// };

// // deprecated
// extern "C"
// {
// DLLEXPORT int32 InitTest(int32 testNumber);
// DLLEXPORT int32 InitUpdatePass(int32 passNumber);
// DLLEXPORT void UpdateStep();

// DLLEXPORT int32 GetTexturesCount();
// DLLEXPORT void PreparePlacedByTextureBuffer();
// DLLEXPORT int32 GetPlacedCount(int32 textureId);
// DLLEXPORT CRect GetPlacedGlyph(int32 textureId, int32 glyphIndex);

// DLLEXPORT void PrepareFreeByTextureBuffer();
// DLLEXPORT int32_2 GetFreeCounts(int32 textureId);
// DLLEXPORT CRect GetFreeShelfRect(int32 textureId, int32 freeShelfIndex);
// DLLEXPORT CRect GetFreeSlotRect(int32 textureId, int32 freeSlotIndex);

// DLLEXPORT int32 InitGetModifiedFreePass();
// DLLEXPORT CRect GetModifiedFreeStep();

// DLLEXPORT int32 InitRemovePlacedPass();
// DLLEXPORT CRect RemovePlacedStep();
// }

// uint16_2 maxTextureDims = {512, 512};
// GlyphAtlasStepped glyphAtlasStepped = GlyphAtlasStepped(maxTextureDims, {1, 200});

// // the containers are flushed after each step.
// std::vector<std::vector<std::pair<FontKey, GlyphKey>>> passKeysByTestNumber{};
// std::vector<std::vector<std::pair<GlyphKey, Glyph>>> placedGlyphsByTexture{};

// std::vector<std::vector<Rect>> freeShelvesByTexture{};
// std::vector<std::vector<Rect>> freeSlotsByTexture{};

// void CheckStep();
// void CheckGetPlacedGlyph();
// void CheckRemovePlaced();
// void CheckGetModifiedFree();
// void CheckGetFreeFromBuffer();

// int main()
// {
//    auto passCount = InitTest(0);
//    for (int p = 0; p < passCount; p++)
//    {
//        auto updateStepsCount = InitUpdatePass(p);
//        for (machine s = 0; s < updateStepsCount; s++)
//        {
//            CheckStep();
//        }

//        CheckRemovePlaced();
//        CheckGetModifiedFree();
//        CheckGetFreeFromBuffer();
//    }
// }

// void CheckStep()
// {
//    UpdateStep();
//    CheckGetModifiedFree();
//    CheckGetPlacedGlyph();
//    CheckGetFreeFromBuffer();
// }

// void CheckRemovePlaced()
// {
//    auto removePlacedStepsCount = InitRemovePlacedPass();
//    for (int i = 0; i < removePlacedStepsCount; i++)
//    {
//        RemovePlacedStep();
//    }
// }

// void CheckGetModifiedFree()
// {
//    auto getModifiedFreeStepsCount = InitGetModifiedFreePass();
//    for (int i = 0; i < getModifiedFreeStepsCount; i++)
//    {
//        GetModifiedFreeStep();
//    }
// }

// void CheckGetFreeFromBuffer()
// {
//    PrepareFreeByTextureBuffer();
//    int32 texturesCount = GetTexturesCount();
//    for (int32 t = 0; t < texturesCount; t++)
//    {
//        int32_2 freeCounts = GetFreeCounts(t);
//        for (int32 r = 0; r < freeCounts.x; r++)
//        {
//            GetFreeShelfRect(t, r);
//        }
//        for (int32 r = 0; r < freeCounts.y; r++)
//        {
//            GetFreeSlotRect(t, r);
//        }
//    }
// }

// void CheckGetPlacedGlyph()
// {
//    int32 texturesCount = GetTexturesCount();
//    PreparePlacedByTextureBuffer();
//    for (int32 t = 0; t < texturesCount; t++)
//    {
//        int32 placedCount = GetPlacedCount(t);
//        for (int32 r = 0; r < placedCount; r++)
//        {
//            GetPlacedGlyph(t, r);
//        }
//    }
// }

// int32 InitTest(int32 testNumber)
// {
//    passKeysByTestNumber = ReadGlyphTestData(GetTestGlyphKeysPath(testNumber));
//    return static_cast<int>(passKeysByTestNumber.size());
// }

// int32 InitUpdatePass(int32 passNumber)
// {
//    auto keys = passKeysByTestNumber[passNumber];
//    glyphAtlasStepped.InitPass(keys);
//    return static_cast<int>(keys.size());
// }

// void UpdateStep()
// {
// //    glyphAtlasStepped.RecordFreeShelfSlotSpace();
//    glyphAtlasStepped.UpdateStep();
// }

// int32 InitRemovePlacedPass()
// {
//    return static_cast<int32>(glyphAtlasStepped.InitRemovePlacesPass());
// }

// CRect RemovePlacedStep()
// {
//    return glyphAtlasStepped.RemovePlacedStep();
// }

// int32 InitGetModifiedFreePass()
// {
//    return static_cast<int32>(glyphAtlasStepped.InitGetModifiedFreePass());
// }

// CRect GetModifiedFreeStep()
// {
//    return glyphAtlasStepped.GetModifiedFreeStep();
// }

// int32 GetTexturesCount()
// {
//    return static_cast<int32>(glyphAtlasStepped.GetSteppedTexturesCount());
// }

// void PreparePlacedByTextureBuffer()
// {
//    machine texturesCount = glyphAtlasStepped.GetSteppedTexturesCount();

//    placedGlyphsByTexture.clear();
//    for (machine i = 0; i < texturesCount; i++)
//    {
//        auto textureGlyphsMap = glyphAtlasStepped.GetGlyphsFromTexture(i);
//        std::vector<std::pair<GlyphKey, Glyph>> textureGlyphs;
//        textureGlyphs.reserve(textureGlyphsMap.size());
//        for (auto pair: textureGlyphsMap)
//        {
//            textureGlyphs.emplace_back(pair);
//        }
//        placedGlyphsByTexture.push_back(textureGlyphs);
//    }
// }

// void PrepareFreeByTextureBuffer()
// {
//    machine texturesCount = glyphAtlasStepped.GetSteppedTexturesCount();

//    freeShelvesByTexture.clear();
//    freeSlotsByTexture.clear();
//    for (machine i = 0; i < texturesCount; i++)
//    {
//        auto freeRects = glyphAtlasStepped.GetFreeShelfSlotSpace(i);
//        freeShelvesByTexture.push_back(freeRects.first);
//        freeSlotsByTexture.push_back(freeRects.second);
//    }
// }

// int32 GetPlacedCount(int32 textureId)
// {
//    return static_cast<int32>(placedGlyphsByTexture[textureId].size());
// }

// int32_2 GetFreeCounts(int32 textureId)
// {
//    return {
//            static_cast<int32>(freeShelvesByTexture[textureId].size()),
//            static_cast<int32>(freeSlotsByTexture[textureId].size())
//    };
// }

// CRect GetPlacedGlyph(int32 textureId, int32 glyphIndex)
// {
//    return placedGlyphsByTexture[textureId][glyphIndex].second.rect;
// }

// CRect GetFreeShelfRect(int32 textureId, int32 freeShelfIndex)
// {
//    return freeShelvesByTexture[textureId][freeShelfIndex];
// }

// CRect GetFreeSlotRect(int32 textureId, int32 freeSlotIndex)
// {
//    return freeSlotsByTexture[textureId][freeSlotIndex];
// }
