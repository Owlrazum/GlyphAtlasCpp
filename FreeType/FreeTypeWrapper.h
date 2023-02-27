#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include <string>

class FreeTypeWrapper
{
public:
    FreeTypeWrapper();
    ~FreeTypeWrapper();

    void UseFont(const char *fontPath, int faceIndex);
    void SetPixelSize(int size);
    FT_Bitmap RenderGlyph(char character);
private:
    FT_Library library;
    FT_Face face;

    FT_Error errorCode;
};