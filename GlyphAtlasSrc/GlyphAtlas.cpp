#include "GlyphAtlas.h"

#include "FileUtils.h"
#include "Algorithms.h"

void GlyphAtlas::Update(std::vector<std::pair<FontKey, GlyphKey>> &updateKeys)
{
    UpdateUnusedCounts();
    for (int i = 0; i < updateKeys.size(); i++)
    {
        freeTypeWrapper.InitGlyphKey(updateKeys[i].first, updateKeys[i].second);
        if (currentFrameUsedGlyphs.find(updateKeys[i].second) != currentFrameUsedGlyphs.end()) // remove duplicates in input
        {
            updateKeys.erase(updateKeys.begin() + i);
            i--;
        }

        currentFrameUsedGlyphs.insert(updateKeys[i].second);
        if (placedGlyphs.find(updateKeys[i].second) != placedGlyphs.end()) // if already contained, skip
        {
            updateKeys.erase(updateKeys.begin() + i);
            i--;
        }
    }

    if (updateKeys.empty())
    {
        return;
    }

    auto updateGlyphs = InitGlyphsDims(updateKeys);
    PlaceWithoutTextureCreation(updateGlyphs);
    if (!updateGlyphs.empty())
    {
        FreeSpaceForNewPlacements();
        PlaceWithTextureCreation(updateGlyphs);
    }

    Render();
}

// todo: try to improve performance by finding freetype function to compute bbox
std::vector<std::pair<GlyphKey, Glyph>> GlyphAtlas::InitGlyphsDims(std::vector<std::pair<FontKey, GlyphKey>> &updateKeys)
{
    std::vector<std::pair<GlyphKey, Glyph>> updateGlyphs;
    updateGlyphs.reserve(updateKeys.size());
    for (auto& pair : updateKeys)
    {
        FontKey& fontKey = pair.first;
        GlyphKey& glyphKey = pair.second;
        Glyph glyph;
        if (auto search = bitmaps.find(glyphKey); search != bitmaps.end())
        {
            InitGlyphDims(glyph, search->second.dims);
        }
        else
        {
            auto ftBitmap = freeTypeWrapper.RenderGlyph(glyphKey); // update GlyphKey.fontId to be in its primary meaning
            GlyphBitmap bitmap (ftBitmap);
            bitmaps.insert(std::make_pair(glyphKey, bitmap));
            InitGlyphDims(glyph, bitmap.dims);
        }

        updateGlyphs.emplace_back(glyphKey, glyph);
    }

    return updateGlyphs;
}

void GlyphAtlas::InitGlyphDims(Glyph &glyph, uint16_2 dims) const
{
    if (hasSinglePixelPadding)
    {
        glyph.rect.w = dims.x + 2;
        glyph.rect.h = dims.y + 2;
    }
    else
    {
        glyph.rect.w = dims.x;
        glyph.rect.h = dims.y;
    }
}


void GlyphAtlas::PlaceWithoutTextureCreation(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs)
{
    UpdateDelimiters(updateGlyphs);

    machine textureIndex = 0;
    while (!updateGlyphs.empty())
    {
        if (textureIndex == textures.size())
        {
            return;
        }
        auto& texture = textures[textureIndex];
        texture.Update(updateGlyphs, placedGlyphs);

        textureIndex++;
    }
}

void GlyphAtlas::PlaceWithTextureCreation(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs)
{
    machine textureIndex = 0;
    while (!updateGlyphs.empty())
    {
        if (textureIndex == textures.size())
        {
            textures.emplace_back( shelfDelimiters, slotDelimiters,
                    textureMaxDims, unusedThresholds, static_cast<uint8>(textures.size()));
        }
        auto& texture = textures[textureIndex];
        texture.Update(updateGlyphs, placedGlyphs);

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

void GlyphAtlas::Render()
{
    for (auto& texture : textures)
    {
        texture.Render(bitmaps, placedGlyphs, hasSinglePixelPadding);
    }
}

void GlyphAtlas::UpdateUnusedCounts()
{
    std::vector<std::pair<GlyphKey, uint16>> unusedKeys;

    std::set_difference(glyphsUnusedFramesCount.begin(), glyphsUnusedFramesCount.end(),
                        currentFrameUsedGlyphs.begin(), currentFrameUsedGlyphs.end(),
                        std::inserter(unusedKeys, unusedKeys.begin()), KeyLess());

    currentFrameUsedGlyphs.clear();

    for (auto& unusedKeyPair : unusedKeys)
    {
        auto& unusedFrameCount = glyphsUnusedFramesCount[unusedKeyPair.first];
        unusedFrameCount++;
        if (unusedFrameCount >= unusedThresholds.y)
        {
            auto textureId = placedGlyphs[unusedKeyPair.first].textureId;
            textures[textureId].RemoveGlyph(unusedKeyPair.first, placedGlyphs, hasSinglePixelPadding);
            glyphsUnusedFramesCount.erase(unusedKeyPair.first);
        }
    }
}

void GlyphAtlas::FreeSpaceForNewPlacements()
{
    auto iter = glyphsUnusedFramesCount.begin();
    auto end = glyphsUnusedFramesCount.end();
    while (iter != end)
    {
        if (iter->second >= unusedThresholds.x)
        {
            auto textureId = placedGlyphs[iter->first].textureId;
            textures[textureId].RemoveGlyph(iter->first, placedGlyphs, hasSinglePixelPadding);
        }
        else
        {
            iter++;
        }
    }
}

Glyph GlyphAtlas::GetPlacedGlyph(GlyphKey key)
{
    if (hasSinglePixelPadding)
    {
        Glyph glyph = placedGlyphs[key];
        glyph.rect.x++;
        glyph.rect.y++;
        glyph.rect.w -= 2;
        glyph.rect.h -= 2;
        return glyph;
    }

    return placedGlyphs[key];
}