#pragma once

#include "Rect.h"
#include "Glyph.h"
#include "GlyphKey.h"
#include "GlyphBitmap.h"
#include "GlyphTexture.h"
#include "FontKey.h"
#include "FreeTypeWrapper.h"

#include <vector>
#include <map>

class GlyphAtlas
{
public:
    explicit GlyphAtlas(uint16_2 textureMaxDimsArg)
            : textureMaxDims(textureMaxDimsArg)
    {
        textures = std::vector<GlyphTexture>();
        bitmaps = std::map<GlyphKey, GlyphBitmap>();
    };

    virtual ~GlyphAtlas() // it is not RAII way, but turned out simpler for me, than to properly define GlyphBitmap class.
    {
        for (auto bitmap: bitmaps)
        {
           delete []bitmap.second.buffer;
        }
    }

    void Update(std::vector<std::pair<FontKey, GlyphKey>> &updateKeys);

    uint32 GetTexturesCount() { return textures.size(); }
    uint8* GetTextureBuffer(machine textureId){ return textures[textureId].GetRawBuffer(); }

protected:
    std::vector<std::pair<GlyphKey, Glyph>> InitGlyphDims(std::vector<std::pair<FontKey, GlyphKey>> &updateKeys);
    void PlaceIfAbsent(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs);
    void RemoveUnused();
    void Render();

    uint16_2 textureMaxDims;
    FreeTypeWrapper freeTypeWrapper;

    std::vector<GlyphTexture> textures; // perhaps one glyphTexture can be referenced by multiple fonts

    std::map<GlyphKey, GlyphBitmap> bitmaps; // only primary meaning of GlyphKey.fontId should be used here.

    std::vector<uint16> shelfDelimiters;
    std::vector<uint16> slotDelimiters;

    void UpdateDelimiters(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs);
};