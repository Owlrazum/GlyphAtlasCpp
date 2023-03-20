#include "CppExport.h"
#include "FreeTypeWrapper.h"
#include "GlyphKey.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

struct GlyphData
{
    uint8 *bitmap;
    int32 width;
    int32 rowCount;
};

FreeTypeWrapper freeType;

extern "C"
{
    DLLEXPORT void RenderCharTest(GlyphData *data, uint32 character, int32 size)
    {
        FontKey fontKey {2, 0, size * 64};
        GlyphKey glyphKey = {0, character};
        freeType.InitGlyphKey(fontKey, glyphKey);
        auto bitmap = freeType.RenderGlyph(glyphKey);
        data->bitmap = bitmap.buffer;
        data->width = static_cast<int32>(bitmap.width);
        data->rowCount = static_cast<int32>(bitmap.rows);
    }
}

int main()
{
    GlyphData data;
    RenderCharTest(&data, 'z', 16);
}