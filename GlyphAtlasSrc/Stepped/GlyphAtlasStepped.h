#pragma once

#include "GlyphAtlas.h"
#include "GlyphTextureStepped.h"
#include <queue>

struct UpdateStatus
{
    bool hasMoreUpdates;
    uint8 textureId;
    Rect placedGlyphRect;
    std::vector<Rect> oldFreeSlots;
    std::vector<Rect> oldFreeShelf;
    std::vector<Rect> newFreeSlots;
    std::vector<Rect> newFreeShelves;
};

// it is broken due to API change. Will revive it only in case of thorough debug needed.

class GlyphAtlasStepped : public GlyphAtlas
{
public:
    explicit GlyphAtlasStepped(uint16_2 textureMaxDimsArg, uint16_2 unusedThresholdsArg, bool hasSinglePixelPadding = true)
            : GlyphAtlas(textureMaxDimsArg, unusedThresholdsArg, hasSinglePixelPadding)
    {
        stepIndex = 0;
    };

    void InitPass(std::vector<std::pair<FontKey, GlyphKey>> &updateKeys);
    // returns whether more steps are left
    UpdateStatus UpdateStep();

    uint32 GetSteppedTexturesCount()
    {
        return steppedTextures.size();
    }

private:
    bool shouldCreateTextures;
    machine stepIndex;

    std::vector<GlyphTextureStepped> steppedTextures;
    std::vector<std::pair<GlyphKey, Glyph>> queue;

    std::vector<std::vector<GlyphKey>> removePlacedQueue;
    std::vector<Rect> removeFreeQueue;

    void OnStepTextureIndex(std::pair<GlyphKey, Glyph> &toStep, machine &textureIndex);
    void FreeSpaceForNewPlacementsStepped();
};