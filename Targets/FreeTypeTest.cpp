#include "CppExport.h"
#include "FreeTypeWrapper.h"
#include "GlyphKey.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

struct GlyphData
{
    unsigned char *bitmap;
    int width;
    int rowCount;

    int pitch;
};

FreeTypeWrapper freeType;

extern "C"
{
    DLLEXPORT void RenderCharTest(GlyphData *data, char character)
    {
        FontKey fontKey {0, 0, 16 * 64};
        auto bitmap = freeType.RenderChar(fontKey, character);
        data->bitmap = bitmap.buffer;
        data->width = static_cast<int>(bitmap.width);
        data->rowCount = static_cast<int>(bitmap.rows);
        data->pitch = bitmap.pitch;
    }
}