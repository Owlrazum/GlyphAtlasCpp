#include "CppExport.h"
#include "FreeTypeWrapper.h"
#include "GlyphKey.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

struct GlyphData
{
    uint8 *bitmap;
    uint32 width;
    uint32 rowCount;

    int32 pitch;
};

FreeTypeWrapper freeType;

extern "C"
{
    DLLEXPORT void RenderCharTest(GlyphData *data, int8 character)
    {
        FontKey fontKey {0, 0, 16 * 64};
        auto bitmap = freeType.RenderChar(fontKey, character);
        data->bitmap = bitmap.buffer;
        data->width = bitmap.width;
        data->rowCount = bitmap.rows;
        data->pitch = bitmap.pitch;
    }
}