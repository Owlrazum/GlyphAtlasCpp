#pragma once

#include "Rect.h"
#include "Glyph.h"
#include "GlyphKey.h"
#include "GlyphTexture.h"
#include "FontKey.h"

#include <vector>
#include <map>

class GlyphAtlas
{
public:
    explicit GlyphAtlas(Pair textureMaxDimsArg)
            : textureMaxDims(textureMaxDimsArg)
    {
        stepIndex = 0;
        textures = std::vector<GlyphTexture>();
    };

    void Update(std::vector<std::pair<GlyphKey, Glyph>> updateGlyphs);

// ---- Stepped version: ----

    void InitPass(const std::vector<GlyphKey> &keys);

    int Step(); // returns number of used textures

// ---- SvgWriting: ----

    void Update(const std::vector<GlyphKey> &keys);


    [[nodiscard]] bool ContainsGlyph(const GlyphKey& key) const
    {
        if (std::any_of(textures.begin(), textures.end(),
                        [key](const GlyphTexture& t){ return t.ContainsGlyph(key); }))
        {
            return true;
        }
        return false;
    }

    [[nodiscard]] bool MarkIfContainsGlyph(const GlyphKey& key)
    {
        for (auto texture : textures)
        {
            if (texture.ContainsGlyph(key))
            {
                texture.MarkGlyph(key);
            }
        }
    }

    [[nodiscard]] Glyph GetGlyph(GlyphKey key);

    int GetPlacedGlyphsCount() const
    {
        int placedGlyphsCount = 0;
        for (auto t : textures)
        {
            placedGlyphsCount += t.GetGlyphCount();
        }
        return placedGlyphsCount;
    }

    [[nodiscard]] const std::vector<std::pair<GlyphKey, Glyph>> GetGlyphsFromTexture(int textureId) const
    {
        return textures[textureId].GetGlyphs();
    }

    [[nodiscard]] const std::pair<std::vector<Rect>, std::vector<Rect>>
    GetFreeShelfSlotSpace(int textureId) const // the first vector is freeSpace for the shelves
    {
        return textures[textureId].GetFreeShelfSlotSpace();
    }

    int GetTextureCount()
    {
        return static_cast<int>(textures.size());
    }

private:
    // ---- Stepped version ----
    int stepIndex;
    std::vector<std::pair<GlyphKey, Glyph>> queue;

    // ---- SvgWriting ----
    Pair textureMaxDims;

    std::vector<GlyphTexture> textures; // perhaps one glyphTexture can be referenced by multiple fonts

    std::vector<ushort> shelfDelimiters;
    std::vector<ushort> slotDelimiters;

    void UpdateDelimiters(const std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs);

    static bool CompareByHeight(const std::pair<GlyphKey, Glyph> &lhs, const std::pair<GlyphKey, Glyph> &rhs)
    {
        return lhs.second.rect.h > rhs.second.rect.h;
    }
};