#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

struct GlyphBitmap
{
    uint16_2 dims;
    uint8* buffer;

    explicit GlyphBitmap(FT_Bitmap freeTypeBitmap)
    {
        dims.x = static_cast<uint16>(freeTypeBitmap.width);
        dims.y = static_cast<uint16>(freeTypeBitmap.rows);
        buffer = new uint8[dims.x * dims.y];
        for (int i = 0; i < dims.y; i++)
        {
            memcpy(buffer + i * dims.x, freeTypeBitmap.buffer + i * freeTypeBitmap.pitch, dims.x);
        }
    }
};
