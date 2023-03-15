#include "GlyphAtlas.h"

#include "FileUtils.h"
#include "Algorithms.h"
#include <iostream>

void GlyphAtlas::Update(std::vector<std::pair<FontKey, GlyphKey>> &updateKeys)
{
    auto updateGlyphs = InitGlyphDims(updateKeys);
    PlaceIfAbsent(updateGlyphs);
    RemoveUnused();
    Render();
}

// todo: try to improve performance by finding freetype function to compute bbox
std::vector<std::pair<GlyphKey, Glyph>> GlyphAtlas::InitGlyphDims(std::vector<std::pair<FontKey, GlyphKey>> &updateKeys)
{
    std::vector<std::pair<GlyphKey, Glyph>> updateGlyphs;
    updateGlyphs.reserve(updateKeys.size());
    for (auto& pair : updateKeys)
    {
        FontKey& fontKey = pair.first;
        GlyphKey& glyphKey = pair.second;
        freeTypeWrapper.InitGlyphKey(fontKey, glyphKey);
        Glyph glyph;
        if (auto search = bitmaps.find(glyphKey); search != bitmaps.end())
        {
            glyph.rect.w = search->second.dims.x;
            glyph.rect.h = search->second.dims.y;
        }
        else
        {
            auto ftBitmap = freeTypeWrapper.RenderGlyph(glyphKey); // updates GlyphKey.fontId to be in its primary meaning
            GlyphBitmap bitmap (ftBitmap);
            bitmaps.insert(std::make_pair(glyphKey, bitmap));
            glyph.rect.w = bitmap.dims.x;
            glyph.rect.h = bitmap.dims.y;
        }

        updateGlyphs.emplace_back(glyphKey, glyph);
    }

    return updateGlyphs;
}


void GlyphAtlas::PlaceIfAbsent(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs)
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

void GlyphAtlas::RemoveUnused()
{
    for (machine i = 0; i < textures.size(); i++)
    {
        textures[i].RemoveUnused();
        if (textures[i].IsEmpty())
        {
            textures.erase(textures.begin() + i);
        }
    }
}

void GlyphAtlas::Render()
{
    for (auto& texture : textures)
    {
        texture.Render(bitmaps);
    }
}