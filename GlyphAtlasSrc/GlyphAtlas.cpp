#include "GlyphAtlas.h"
#include "FileUtils.h"

#include <iostream>

// Currently renders and discards buffer.
// todo: try to improve performance by finding freetype function to compute bbox
void GlyphAtlas::InitGlyphDims(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs)
{
    for (auto& pair : updateGlyphs)
    {
        freeTypeWrapper.AddFont(pair.first.fontIndex);
        auto bitmap = freeTypeWrapper.RenderGlyph(pair.first);
        pair.second.rect.w = bitmap.pitch;
        pair.second.rect.h = bitmap.rows;
    }
}


void GlyphAtlas::Update(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs)
{
    std::sort(updateGlyphs.begin(), updateGlyphs.end(), CompareByHeight);
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

    for (auto & texture : textures)
    {
        texture.RemoveUnused();
    }
}

// todo: improve by developing an algorithm for determining delimiters
void GlyphAtlas::UpdateDelimiters(const std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs)
{
    if (shelfDelimiters.empty() || slotDelimiters.empty())
    {
        shelfDelimiters = {16, 32, 64};
        slotDelimiters = {16, 32, 48, 64, 96};
    }
}

void GlyphAtlas::Render()
{
    for (auto& texture : textures)
    {
        texture.Render(freeTypeWrapper);
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
//void GlyphAtlas::InitPass(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs)
//{
//    ReadGlyphRects(updateGlyphs);
//    std::sort(queue.begin(), queue.end(), CompareByHeight);
//    UpdateDelimiters(queue);
//    stepIndex = 0;
//}

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