#pragma once

#include "Math/Rect.h"
#include "Glyph.h"
#include "GlyphKey.h"
#include "GlyphTexture.h"
#include "FontKey.h"

#include <vector>
#include <map>

class GlyphAtlas
{
public:
    GlyphAtlas(
            const std::vector<ushort> &shelfDelimitersArg,
            const std::vector<ushort> &slotDelimiters,
            Pair textureMaxDimsArg)
            : shelfDelimiters(shelfDelimitersArg),
              slotDelimiters(slotDelimiters),
              textureMaxDims(textureMaxDimsArg)
    {
        textures = std::vector<GlyphTexture>();
    };

// ---- Stepped version: ----

    int InitPass(const std::vector<GlyphKey> &keys);
    int Step();

// ---- SvgWriting: ----

    void Update(const std::vector<GlyphKey> &keys);

    [[nodiscard]] const std::vector<std::pair<GlyphKey, Glyph>> GetGlyphsFromTexture(int textureId) const;

    [[nodiscard]] const std::pair<std::vector<Rect>, std::vector<Rect>>
    GetFreeShelfSlotSpace(int textureId) const; // the first vector is freeSpace for the shelves

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

    const std::vector<ushort> shelfDelimiters;
    const std::vector<ushort> slotDelimiters;

    static bool CompareByHeight(const std::pair<GlyphKey, Glyph> &lhs, const std::pair<GlyphKey, Glyph> &rhs);
};