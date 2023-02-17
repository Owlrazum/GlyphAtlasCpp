#pragma once

#include "FontKey.h"

#include <vector>
#include <string>
#include <sstream>

// Update queue consists of
// Struct {
//  FontKey
//  GlyphID - integer
// }

// One shelf one fontKey ?
// The cost of restructuring
// for each font for each size max height for the shelf
// delimiters may be calculated based on each font


// 1) Remove already contained from the input queue
// 2) Figure if existing textures can contain all of the input queue
//    a. One font - one texture if no room, if there is space, multiple fonts inside texture. Mode of mixed or not
//    b.

struct GlyphKey
{
    ushort fontId;
    ushort glyphId;

    bool operator<(const GlyphKey &rhs) const
    {
        if (fontId < rhs.fontId)
            return true;
        if (rhs.fontId < fontId)
            return false;
        return glyphId < rhs.glyphId;
    }

    bool operator>(const GlyphKey &rhs) const
    {
        return rhs < *this;
    }

    bool operator<=(const GlyphKey &rhs) const
    {
        return !(rhs < *this);
    }

    bool operator>=(const GlyphKey &rhs) const
    {
        return !(*this < rhs);
    }
};