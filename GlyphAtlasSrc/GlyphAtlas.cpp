#include "GlyphAtlas.h"
#include "FileUtils.h"

#include <iostream>

// todo: improve by developing an algorithm for determining delimiters
void GlyphAtlas::UpdateDelimiters(const std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs)
{
    if (shelfDelimiters.size() == 0 || slotDelimiters.size() == 0)
    {
        shelfDelimiters = {};
        slotDelimiters = {};
    }
}

void GlyphAtlas::Update(std::vector<std::pair<GlyphKey, Glyph>> updateGlyphs)
{
    std::sort(updateGlyphs.begin(), updateGlyphs.end(), CompareByHeight);
    UpdateDelimiters(updateGlyphs);

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

Glyph GlyphAtlas::GetGlyph(GlyphKey key)
{
    Glyph glyph;
    for (auto t : textures)
    {
        if (t.GetGlyph(key, glyph))
        {
            return glyph;
        }
    }

    throw std::out_of_range("The GlyphKey does not exists in the atlas!");
}

void GlyphAtlas::InitPass(const std::vector<GlyphKey> &keys)
{
    queue = ReadGlyphs(keys);
    std::sort(queue.begin(), queue.end(), CompareByHeight);
    UpdateDelimiters(queue);
    stepIndex = 0;
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
    Update(updateGlyphs);
}