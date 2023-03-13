#include "GlyphAtlas.h"

#include "FileUtils.h"
#include "Algorithms.h"
#include <iostream>

void GlyphAtlas::UpdateDelimiters(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs)
{
//    default values
//    shelfDelimiters = {16, 32, 64};
//    slotDelimiters = {16, 32, 48, 64, 96};

    if (shelfDelimiters.empty() || slotDelimiters.empty())
    {
        std::tie(shelfDelimiters, slotDelimiters) = CreateDelimitersByDeltas(updateGlyphs);
    }
    else
    {
        UpdateDelimitersByDeltas(updateGlyphs, shelfDelimiters, slotDelimiters);
    }
}

// Currently renders and discards buffer.
// todo: try to improve performance by finding freetype function to compute bbox
void GlyphAtlas::InitGlyphDims(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs)
{
    for (auto& pair : updateGlyphs)
    {
        freeTypeWrapper.AddFont(pair.first.fontIndex);
        if (auto search = bitmaps.find(pair.first); search != bitmaps.end())
        {
            pair.second.rect.w = search->second.dims.x;
            pair.second.rect.h = search->second.dims.y;
        }
        else
        {
            auto ftBitmap = freeTypeWrapper.RenderGlyph(pair.first);
            GlyphBitmap bitmap (ftBitmap);
            bitmaps.insert(std::make_pair(pair.first, bitmap));
            pair.second.rect.w = bitmap.dims.x;
            pair.second.rect.h = bitmap.dims.y;
        }
    }
}


void GlyphAtlas::Update(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs)
{
    UpdateDelimiters(updateGlyphs);

    machine textureIndex = 0;
    while (!updateGlyphs.empty())
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
}

void GlyphAtlas::RemoveUnused()
{
    machine textureIndex = 0;
    for (auto & texture : textures)
    {
        texture.RemoveUnused();
        textureIndex++;
    }
}

void GlyphAtlas::Render()
{
    for (auto& texture : textures)
    {
        texture.Render(bitmaps);
    }
}