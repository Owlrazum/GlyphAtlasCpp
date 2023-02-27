#include "CppExport.h"
#include "FreeTypeWrapper.h"

#include <ft2build.h>
#include FT_FREETYPE_H"freetype/freetype.h"
#include FT_GLYPH_H"freetype/ftglyph.h"

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
    DLLEXPORT void RenderGlyphTest(GlyphData *data, char character)
    {
        freeType.UseFont("/Users/Abai/Desktop/Evolve/GlyphAtlas/Fonts/Lato-Regular.ttf", 0);
        freeType.SetPixelSize(16 * 64);
        auto bitmap = freeType.RenderGlyph(character);
        data->bitmap = bitmap.buffer;
        data->width = bitmap.width;
        data->rowCount = bitmap.rows;
        data->pitch = bitmap.pitch;
    }
}