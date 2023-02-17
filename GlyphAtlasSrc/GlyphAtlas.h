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
            const std::vector<ushort>& widthDelimitersArg,
            Pair textureMaxDimsArg)
            : shelfDelimiters(shelfDelimitersArg),
            widthDelimiters(widthDelimitersArg),
            textureMaxDims(textureMaxDimsArg)
    {
        textures = std::vector<GlyphTexture>();
    };

    void Update(const std::vector<GlyphKey>& keys);
//    GlyphHandle GetGlyph(FontKey key, GlyphId id);

    [[nodiscard]] const std::vector<Glyph> GetGlyphsFromTexture(int textureId) const;

    int GetTextureCount()
    {
        return static_cast<int>(textures.size());
    }

private:
    Pair textureMaxDims;

    int texturesCount{};

    std::vector<GlyphTexture> textures; // perhaps one glyphTexture can be referenced by multiple fonts

    const std::vector<ushort> shelfDelimiters;
    const std::vector<ushort> widthDelimiters;

    static bool CompareByHeight(const std::pair<GlyphKey, Glyph> &lhs, const std::pair<GlyphKey, Glyph> &rhs);
};