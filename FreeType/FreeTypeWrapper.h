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

    void SetDpi(uint16_2 newDpi)
    {
        dpi = newDpi;
    }

    void AddFont(uint8_t fontId)
    {
        AddFont({fontId, 0, defaultFontSize});
    }

    void AddFont(FontKey fontKey);
    FT_Bitmap RenderChar(const FontKey &fontKey, uint32 character);
    FT_Bitmap RenderGlyph(const GlyphKey& key);
private:
    FT_Library library;
    uint16_2 dpi {300, 300};
    std::map<FontFileId, FT_Face> facesByFont;
    std::map<FontKey, FT_Size> sizesByFontKey; // to use one face with different sizes
    std::vector<FT_Bitmap> createdBitmaps;

    FT_Error errorCode;

    const int32 defaultFontSize = 16 * 64;
};