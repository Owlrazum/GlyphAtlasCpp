#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include "FontKey.h"
#include "GlyphKey.h"

#include <string>
#include <map>
#include <vector>

// Todo add public API for adding FontKey in an Evolve context
class FreeTypeWrapper
{
public:
    FreeTypeWrapper();
    ~FreeTypeWrapper();

    void SetDpi(uint16_2 newDpi)
    {
        dpi = newDpi;
    }

    void InitGlyphKey(const FontKey &fontKey, GlyphKey &glyphKey);
    FT_Bitmap RenderGlyph(const GlyphKey& key);

private:
    uint8 AddFontKey(const FontKey &fontKey);

    FT_Library library;
    uint16_2 dpi {220, 220};

    // I needed an indexed set, using method described here:
    // https://stackoverflow.com/questions/2561992/c-need-indexed-set

    std::map<FontKey, uint8> keysToIndexInPointers;
    std::vector<const FontKey*> pointersToKeys;

    // using FontFileId as key instead of FontKey
    // to use one face with different sizes

    std::map<FontFileId, FT_Face> facesByFontFileId;
    std::map<FontKey, FT_Size> sizesByFontKey;

    std::vector<FT_Bitmap> createdBitmaps;

    FT_Error errorCode;

    const int32 defaultFontSize = 22 * 64;
};