#include "SvgWriter.h"

#include "FileUtils.h"

#include <fstream>
#include <string>

SvgWriter::SvgWriter(const std::string &backColor)
{
    std::stringstream ss;
    ss << R"(<svg version="1.1")" << "\n" << indent << R"(xmlns="http://www.w3.org/2000/svg")" << "\n";
    // texture dimensions
    ss << indent << R"(width= "2048" height= "2048">)" << "\n\n";
    // background
    ss << indent << R"(<rect width="100%" height="100%" fill=")" << backColor << "\" />\n";
    opening = ss.str();
}

void SvgWriter::WriteAtlas(GlyphAtlas &atlas)
{
    auto size = atlas.GetTextureCount();
    for (int i = 0; i < size; i++)
    {
        auto glyphs = atlas.GetGlyphsFromTexture(i);
        std::ofstream out {GetTexturePath(i)};
        out << opening;
        for (auto glyph: glyphs)
        {
            WriteGlyphDefault(glyph, out);
        }
        out << svgClose;
    }
}

void SvgWriter::WriteRect(Rect rect, std::ofstream &out, const std::string &color)
{
    out << indent << "<rect x=\"" << rect.x << "\" y=\"" << rect.y
        << "\" width=\"" << rect.w << "\" height=\"" << rect.h
        << "\" fill=\"" << color << "\" /> \n";
}

void SvgWriter::WriteGlyph(Glyph glyph, std::ofstream &out, const std::string &fillColor, ushort borderWidthHalf,
                           const std::string &borderColor)
{
    out << indent << "<rect x=\"" << glyph.rect.x + borderWidthHalf << "\" y=\"" << glyph.rect.y + borderWidthHalf
        << "\" width=\"" << glyph.rect.w - borderWidthHalf * 2 << "\" height=\"" << glyph.rect.h - borderWidthHalf * 2
        << "\" fill=\"" << fillColor
        << "\" stroke-width=\"" << borderWidthHalf * 2
        << "\" stroke=\"" << borderColor << "\"/> \n";
}
