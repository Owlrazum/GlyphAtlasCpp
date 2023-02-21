#include "GlyphAtlas.h"
#include "FileUtils.h"

#include <iostream>

int GlyphAtlas::InitPass(const std::vector<GlyphKey> &keys)
{
    queue = ReadGlyphs(keys);
    std::sort(queue.begin(), queue.end(), CompareByHeight);
    stepIndex = 0;
    return queue.size();
}

int GlyphAtlas::Step()
{
    auto toStep = queue[stepIndex];
    int textureIndex = 0;
    bool wasPlaced = false;
    do
    {
        if (textureIndex == textures.size())
        {
            textures.emplace_back(
                    shelfDelimiters, slotDelimiters,
                    textureMaxDims, textures.size());
        }
        wasPlaced = textures[textureIndex].Step(toStep);
        textureIndex++;
    }
    while(!wasPlaced);

    stepIndex++;
    return static_cast<int> (textures.size());
}

void GlyphAtlas::Update(const std::vector<GlyphKey> &keys)
{
    std::vector<std::pair<GlyphKey, Glyph>> updateGlyphs = ReadGlyphs(keys);
    std::sort(updateGlyphs.begin(), updateGlyphs.end(), CompareByHeight);

    int textureIndex = 0;
    while (updateGlyphs.size() > 0)
    {
        if (textureIndex == textures.size())
        {
            textures.emplace_back(
                    shelfDelimiters, slotDelimiters,
                    textureMaxDims, textures.size());
        }
        auto& texture = textures[textureIndex];
        texture.Update(updateGlyphs);

        textureIndex++;
    }

    for (auto & texture : textures)
    {
        texture.RemoveUnused();
    }
}

bool GlyphAtlas::CompareByHeight(const std::pair<GlyphKey, Glyph> &lhs, const std::pair<GlyphKey, Glyph> &rhs)
{
    return lhs.second.rect.h > rhs.second.rect.h;
}

const std::vector<std::pair<GlyphKey, Glyph>> GlyphAtlas::GetGlyphsFromTexture(int textureId) const
{
    return textures[textureId].GetGlyphs();
}

const std::pair<std::vector<Rect>, std::vector<Rect>> GlyphAtlas::GetFreeShelfSlotSpace(int textureId) const
{
    return textures[textureId].GetFreeShelfSlotSpace();
}