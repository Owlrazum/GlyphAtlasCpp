#pragma once

#include "Rect.h"
#include "Glyph.h"
#include "GlyphKey.h"
#include "GlyphTexture.h"
#include "FontKey.h"

#include <vector>
#include <map>

struct GlyphBitmap
{
    uint16_2 dims;
    int32 pitch;
    std::vector<uint8> buffer;

    explicit GlyphBitmap(FT_Bitmap freeTypeBitmap)
    {
        dims.x = static_cast<uint16>(freeTypeBitmap.width);
        dims.y = static_cast<uint16>(freeTypeBitmap.rows);
        pitch = freeTypeBitmap.pitch;
        buffer.reserve(dims.x * dims.y);
        auto ftBuffer = freeTypeBitmap.buffer;
        for (int i = 0; i < dims.y; i++)
        {
            memcpy(buffer.data() + i * pitch, ftBuffer + i * pitch, dims.x);
        }
    }
};

class GlyphAtlas
{
public:
    explicit GlyphAtlas(uint16_2 textureMaxDimsArg)
            : textureMaxDims(textureMaxDimsArg)
    {
        stepIndex = 0;
        textures = std::vector<GlyphTexture>();
        bitmaps = std::map<GlyphKey, GlyphBitmap>();
    };

    void InitGlyphDims(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs);
    void Update(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs);

// ---- Stepped version: ----
    void InitPass(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs);
    machine Step(); // returns number of used textures
    void Render();

// ---- Getters: ----

    uint8* GetTextureBuffer(machine textureId)
    {
        return textures[textureId].GetRawBuffer();
    }

    [[nodiscard]] bool ContainsGlyph(const GlyphKey& key) const;
    [[nodiscard]] bool MarkIfContainsGlyph(const GlyphKey& key);
    [[nodiscard]] Glyph GetGlyph(GlyphKey key);
    [[nodiscard]] machine GetPlacedGlyphsCount() const;

    [[nodiscard]] std::map<GlyphKey, Glyph> GetGlyphsFromTexture(machine textureId)
        { return textures[textureId].GetGlyphs(); }
    [[nodiscard]] std::pair<std::vector<Rect>, std::vector<Rect>> GetFreeShelfSlotSpace(machine textureId) const // the first vector is freeSpace for the shelves
        { return textures[textureId].GetFreeShelfSlotSpace(); }
    machine GetTexturesCount()
        { return static_cast<int>(textures.size()); }

private:
    uint16_2 textureMaxDims;
    FreeTypeWrapper freeTypeWrapper;

    std::vector<GlyphTexture> textures; // perhaps one glyphTexture can be referenced by multiple fonts

    std::map<GlyphKey, GlyphBitmap> bitmaps;


    std::vector<uint16> shelfDelimiters;
    std::vector<uint16> slotDelimiters;

    void UpdateDelimiters(const std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs);

    static bool CompareByHeight(const std::pair<GlyphKey, Glyph> &lhs, const std::pair<GlyphKey, Glyph> &rhs)
    {
        return lhs.second.rect.h > rhs.second.rect.h;
    }

    // ---- Stepped version ----
    machine stepIndex;
    std::vector<std::pair<GlyphKey, Glyph>> queue;
};