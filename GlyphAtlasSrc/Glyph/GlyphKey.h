#pragma once

#include "FontKey.h"

#include <vector>
#include <string>
#include <sstream>

// toreview:
// 1. One font - one texture if no room, if there is space, multiple fonts inside texture. Mode of mixed or not
// 2. One shelf one fontIndex ?
// 3. for each font for each size max height for the shelf - forgot what it means
// 4. The cost of restructuring
// 5. delimiters may be calculated based on each font

struct GlyphKey
{
    uint8 fontIndex; // the FreeTypeWrapper initializes this field as an index to internal FontKeys data structure.
    uint32 character;

    [[nodiscard]] std::string ToString() const
    {
        return std::to_string(fontIndex) + ":" + std::to_string(character);
    }

    bool operator<(const GlyphKey &rhs) const
    {
        if (fontIndex < rhs.fontIndex)
            return true;
        if (rhs.fontIndex < fontIndex)
            return false;
        return character < rhs.character;
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