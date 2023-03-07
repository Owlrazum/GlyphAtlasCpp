#include "GlyphAtlas.h"

#include "FileUtils.h"
#include "Algorithms.h"
#include <iostream>

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

    RemoveUnused();
}

void GlyphAtlas::RemoveUnused()
{
    for (auto & texture : textures)
    {
        texture.RemoveUnused();
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

void GlyphAtlas::Render()
{
    for (auto& texture : textures)
    {
        texture.Render(bitmaps);
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

// todo: read glyph rects using freetype
void GlyphAtlas::InitPass(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs)
{
    InitGlyphDims(updateGlyphs);
    queue = updateGlyphs;
    UpdateDelimiters(queue);
    stepIndex = 0;
}

machine GlyphAtlas::Step()
{
    auto toStep = queue[stepIndex];
    machine textureIndex = 0;
    bool wasPlaced;
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

bool GlyphAtlas::ContainsGlyph(const GlyphKey &key) const
{
    if (std::any_of(textures.begin(), textures.end(),
                    [key](const GlyphTexture& t){ return t.ContainsGlyph(key); }))
    {
        return true;
    }
    return false;
}

bool GlyphAtlas::MarkIfContainsGlyph(const GlyphKey &key)
{
    for (auto texture : textures)
    {
        if (texture.ContainsGlyph(key))
        {
            texture.MarkGlyph(key);
            return true;
        }
    }
    return false;
}

machine GlyphAtlas::GetPlacedGlyphsCount() const
{
    machine placedGlyphsCount = 0;
    for (auto t : textures)
    {
        placedGlyphsCount += t.GetGlyphCount();
    }
    return placedGlyphsCount;
}