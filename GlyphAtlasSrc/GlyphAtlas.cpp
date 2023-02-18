#include "GlyphAtlas.h"
#include "FileUtils.h"

#include <iostream>

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
                    shelfDelimiters, widthDelimiters,
                    textureMaxDims, texturesCount++);
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