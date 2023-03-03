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

// One shelf one fontIndex ?
// The cost of restructuring
// for each font for each size max height for the shelf
// delimiters may be calculated based on each font


// 1) Remove already contained from the input queue
// 2) Figure if existing textures can contain all the input queue
//    a) One font - one texture if no room, if there is space, multiple fonts inside texture. Mode of mixed or not

struct GlyphKey
{
    // toask: probably can use char, but do not how to do it correctly, since it would read character, not number
    uint8 fontIndex;

    char32_t glyphIndex; // not sure whether char32_t is the correct type, but uint32 is causing errors.

    [[nodiscard]] std::string ToString() const
    {
        return std::to_string(fontIndex) + ":" + std::to_string(glyphIndex);
    }

    bool operator<(const GlyphKey &rhs) const
    {
        if (fontIndex < rhs.fontIndex)
            return true;
        if (rhs.fontIndex < fontIndex)
            return false;
        return glyphIndex < rhs.glyphIndex;
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