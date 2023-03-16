#include "GlyphAtlasStepped.h"

#include "FileUtils.h"
#include <iostream>

// toreview: try to optimize freeType dimensions getting using Matt's advice
void GlyphAtlasStepped::InitPass(std::vector<std::pair<FontKey, GlyphKey>> &updateKeys)
{
    UpdateUnusedCounts();
    for (int i = 0; i < updateKeys.size(); i++)
    {
        if (placedGlyphs.find(updateKeys[i].second) != placedGlyphs.end())
        {
            updateKeys.erase(updateKeys.begin() + i);
            currentFrameUsedGlyphs.insert(updateKeys[i].second);
            i--;
        }
    }

    queue = InitGlyphsDims(updateKeys);

    UpdateDelimiters(queue);
    if (steppedTextures.empty())
    {
        steppedTextures.emplace_back(
                shelfDelimiters, slotDelimiters,
                textureMaxDims, unusedThresholds, steppedTextures.size());
    }
}

bool GlyphAtlasStepped::UpdateStep()
{
    if (queue.empty())
    {
        throw std::out_of_range("redundant update step");
    }

    auto& toStep = queue[stepIndex];
    machine textureIndex = -1;
    do
    {
        OnStepTextureIndex(toStep, textureIndex);
        bool wasPlaced = steppedTextures[textureIndex].UpdateStep(toStep);
        if (wasPlaced)
        {
            if (queue.empty())
            {
                return false;
            }
            return true;
        }
    }
    while(textureIndex < 1000);
}

void GlyphAtlasStepped::OnStepTextureIndex(std::pair<GlyphKey, Glyph> &toStep, machine &textureIndex)
{
    textureIndex++;
    if (textureIndex == steppedTextures.size())
    {
        if (shouldCreateTextures)
        {
            steppedTextures.emplace_back(
                    shelfDelimiters, slotDelimiters,
                    textureMaxDims, unusedThresholds, steppedTextures.size());
        }
        else
        {
            stepIndex++;
            if (stepIndex == queue.size())
            {
                stepIndex = 0;
                shouldCreateTextures = true;
                FreeSpaceForNewPlacements();
            }
            toStep = queue[stepIndex];
            textureIndex = 0;
        }
    }
}

uint32 GlyphAtlasStepped::InitRemovePlacesPass()
{
    uint32 unusedCount = 0;
    removePlacedQueue.clear();
    for(auto &t : steppedTextures)
    {
        auto v = t.CollectUnused();
        unusedCount += v.size();
        removePlacedQueue.push_back(v);
    }
    return unusedCount;
}

CRect GlyphAtlasStepped::RemovePlacedStep()
{
    for(int i = 0; i < removePlacedQueue.size(); i++)
    {
        if (removePlacedQueue[i].empty())
        {
            continue;
        }

        GlyphKey keyToRemove = *(removePlacedQueue[i].end() - 1);
        auto rect = steppedTextures[i].RemoveStep(keyToRemove);
        if (steppedTextures[i].IsEmpty())
        {
            steppedTextures.erase(steppedTextures.begin() + i);
        }
        removePlacedQueue[i].erase(removePlacedQueue[i].end() - 1);

        return rect;
    }

    throw std::out_of_range("Remove step did not find what to remove");
}

uint32 GlyphAtlasStepped::InitGetModifiedFreePass()
{
    uint32 unusedCount = 0;
    removeFreeQueue.clear();
    for(auto &t : steppedTextures)
    {
        auto v = t.GetModifiedFreeRects();
        unusedCount += v.size();
        removeFreeQueue.insert(removeFreeQueue.end(), v.begin(), v.end());
    }
    return unusedCount;
}

CRect GlyphAtlasStepped::GetModifiedFreeStep()
{
    if (!removeFreeQueue.empty())
    {
        auto rect = *(removeFreeQueue.end() - 1);
        removeFreeQueue.erase(removeFreeQueue.end() - 1);

        return rect;
    }

    throw std::out_of_range("Remove step did not find what to remove");
}

void GlyphAtlasStepped::FreeSpaceForNewPlacementsStepped()
{
    auto iter = glyphsUnusedFramesCount.begin();
    auto end = glyphsUnusedFramesCount.end();
    while (iter != end)
    {
        if (iter->second >= unusedThresholds.x)
        {
            auto textureId = placedGlyphs[iter->first].textureId;
            steppedTextures[textureId].RemoveGlyph(iter->first, placedGlyphs, hasSinglePixelPadding);
        }
        else
        {
            iter++;
        }
    }
}
