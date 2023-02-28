#include "CppExport.h"
#include "FreeTypeWrapper.h"
#include "GlyphKey.h"
#include "FileUtils.h"
#include "GlyphAtlas.h"

#include <vector>

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
        freeType.UseFont("/Users/Abai/Desktop/Evolve/GlyphAtlas/Fonts/Lato-Regular.ttf", 0);
        freeType.SetPixelSize(16 * 64);
        auto bitmap = freeType.RenderChar(character);
        data->bitmap = bitmap.buffer;
        data->width = bitmap.width;
        data->rowCount = bitmap.rows;
        data->pitch = bitmap.pitch;
    }
}