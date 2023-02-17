#pragma once

#include "Rect.h"
#include "Colors.h"
#include "Glyph.h"
#include "GlyphAtlas.h"

#include <string>

class SvgWriter
{
public:
    SvgWriter(const std::string& backColor);

    void WriteAtlas(GlyphAtlas &atlas);

private:
    std::string opening;

    static void WriteGlyphDefault(Glyph glyph, std::ofstream &out)
    { WriteGlyph(glyph, out, cream, 1, burntOrange);}
    static void WriteGlyph(Glyph glyph, std::ofstream &out, const std::string &fillColor, ushort borderWidthHalf,
                    const std::string &borderColor);
    static void WriteRect(Rect rect, std::ofstream& out, const std::string& color);

    inline static const std::string indent = "    ";
    inline static const std::string svgClose = "\n</svg>";

//    const std::ofstream::openmode appendMode = std::ofstream::out | std::ofstream::app;
//    const std::ofstream::openmode truncMode = std::ofstream::out | std::ofstream::trunc;
};