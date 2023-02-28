#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include "FontKey.h"

#include <string>
#include <map>
#include <vector>

class FreeTypeWrapper
{
public:
    FreeTypeWrapper();
    ~FreeTypeWrapper();

    void AddFont(FontKey fontKey);
    FT_Bitmap RenderChar(FontKey fontKey, char character);
    FT_Bitmap RenderGlyph(FontKey fontKey, int glyphIndex);
private:
    FT_Library library;
    std::map<FontKey, FT_Face> facesByFont;
    std::vector<FT_Bitmap> createdBitmaps;

    FT_Error errorCode;
};