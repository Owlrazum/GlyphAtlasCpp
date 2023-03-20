#pragma once

#include "simdutf.h"
#include "GlyphKey.h"

class SimdUtfWrapper
{
public:
    // throws on conversion and validation errorrs
    static void ConvertToUtf32(GlyphKey &glyphKey);
};