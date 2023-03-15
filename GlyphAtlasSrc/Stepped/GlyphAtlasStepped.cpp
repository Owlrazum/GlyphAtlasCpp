#include "GlyphAtlasStepped.h"

#include "FileUtils.h"
#include <iostream>

// toreview: try to optimize freeType dimensions getting using Matt's advice
void GlyphAtlasStepped::InitPass(std::vector<std::pair<FontKey, GlyphKey>> &updateKeys)
{
    queue = InitGlyphDims(updateKeys);
    UpdateDelimiters(queue);
    stepIndex = 0;
}

void GlyphAtlasStepped::UpdateStep()
{
    auto toStep = queue[stepIndex];
    machine textureIndex = 0;
    bool wasPlaced;
    do
    {
        if (textureIndex == steppedTextures.size())
        {
            steppedTextures.emplace_back(
                    shelfDelimiters, slotDelimiters,
                    textureMaxDims, steppedTextures.size());
        }
        wasPlaced = steppedTextures[textureIndex].UpdateStep(toStep);
        textureIndex++;
    }
    while(!wasPlaced);

    stepIndex++;
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