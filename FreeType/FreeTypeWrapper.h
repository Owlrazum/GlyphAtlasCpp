#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include "FontKey.h"
#include "GlyphKey.h"

#include <string>
#include <map>
#include <vector>

class FreeTypeWrapper
{
public:
    FreeTypeWrapper();
    ~FreeTypeWrapper();

    void AddFont(uint8_t fontId)
    {
        AddFont({fontId, 0, defaultFontSize});
    }

    void AddFont(FontKey fontKey);
    FT_Bitmap RenderChar(const FontKey &fontKey, char character);
    FT_Bitmap RenderGlyph(const FontKey &fontKey, int glyphIndex);
    FT_Bitmap RenderGlyph(const GlyphKey& key);
private:
    FT_Library library;
    std::map<FontKey, FT_Face> facesByFont;
    std::vector<FT_Bitmap> createdBitmaps;

    FT_Error errorCode;

    const long defaultFontSize = 16 * 32;
};