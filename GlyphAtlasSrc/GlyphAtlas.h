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
    explicit GlyphAtlas(uint16_2 textureMaxDimsArg, uint16_2 unusedThresholdsArg, bool hasSinglePixelPaddingArg = true)
            : textureMaxDims(textureMaxDimsArg),
              unusedThresholds(unusedThresholdsArg),
              hasSinglePixelPadding(hasSinglePixelPaddingArg)
    {
        textures = std::vector<GlyphTexture>();
        bitmaps = std::map<GlyphKey, GlyphBitmap>();
        placedGlyphs = std::map<GlyphKey, Glyph>();
    };

    virtual ~GlyphAtlas() // it is not RAII way, but turned out simpler for me, than to properly define GlyphBitmap class.
    {
        for (auto bitmap: bitmaps)
        {
           delete []bitmap.second.buffer;
        }
    }

    void Update(std::vector<std::pair<FontKey, GlyphKey>> &updateKeys);
    Glyph GetPlacedGlyph(GlyphKey glyphKey); // do not remove: the public API for Evolve

    uint32 GetTexturesCount() { return textures.size(); }
    uint8* GetTextureBuffer(machine textureId){ return textures[textureId].GetRawBuffer(); }

protected:
    void InitGlyphDims(Glyph &glyph, uint16_2 dims) const;
    std::vector<std::pair<GlyphKey, Glyph>> InitGlyphsDims(std::vector<std::pair<FontKey, GlyphKey>> &updateKeys);
    void PlaceWithoutTextureCreation(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs);
    void PlaceWithTextureCreation(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs);
    void FreeSpaceForNewPlacements();
    void Render();
    void UpdateUnusedCounts();

    uint16_2 textureMaxDims;
    bool hasSinglePixelPadding;

    FreeTypeWrapper freeTypeWrapper;

    std::vector<GlyphTexture> textures;

    std::map<GlyphKey, Glyph> placedGlyphs;
    std::map<GlyphKey, GlyphBitmap> bitmaps;

    std::set<GlyphKey> currentFrameUsedGlyphs;
    std::map<GlyphKey, uint16> glyphsUnusedFramesCount;

    uint16_2 unusedThresholds = {1, 200};

    std::vector<uint16> shelfDelimiters;
    std::vector<uint16> slotDelimiters;

    void UpdateDelimiters(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs);
};