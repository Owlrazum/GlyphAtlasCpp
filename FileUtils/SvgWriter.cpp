#include "SvgWriter.h"

#include "FileUtils.h"

#include <fstream>
#include <sstream>
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

//void SvgWriter::WriteAtlas(GlyphAtlas &atlas)
//{
//    auto size = atlas.GetTexturesCount();
//    for (machine i = 0; i < size; i++)
//    {
//        auto glyphs = atlas.GetGlyphsFromTexture(i);
//        auto freeRects = atlas.GetFreeShelfSlotSpace(i);
//        std::ofstream out{GetSvgTexturePath(i)};
//        out << opening;
//        out << "\n";
//        for (auto glyph: glyphs)
//        {
//            WriteGlyphDefault(glyph, out, true);
//        }
//        out << "\n";
//        for (auto rect: freeRects.first)
//        {
//            WriteRect(rect, out, tiffanyBlue, 1, oliveGreen);
//        }
//        for (auto rect: freeRects.second)
//        {
//            WriteRect(rect, out, yellowGreen, 1, coolGray);
//        }
//        out << "\n";
//
//        out << svgClose;
//    }
//}

void SvgWriter::WriteRect(Rect rect, std::ofstream &out, const std::string &color)
{
    out << indent << "<rect x=\"" << rect.x << "\" y=\"" << rect.y
        << "\" width=\"" << rect.w << "\" height=\"" << rect.h
        << "\" fill=\"" << color << "\" /> \n";
}

void SvgWriter::WriteRect(Rect &rect, std::ofstream &out, const std::string &fillColor, uint16 borderWidthHalf,
                          const std::string &borderColor)
{
    out << indent << "<rect x=\"" << rect.x + borderWidthHalf << "\" y=\"" << rect.y + borderWidthHalf
        << "\" width=\"" << rect.w - borderWidthHalf * 2 << "\" height=\"" << rect.h - borderWidthHalf * 2
        << "\" fill=\"" << fillColor
        << "\" stroke-width=\"" << borderWidthHalf * 2
        << "\" stroke=\"" << borderColor << "\"/> \n";
}

static void SvgWriter::WriteGlyphDefault(std::pair<GlyphKey, Glyph> glyph, std::ofstream &out, bool writeKey)
{
    Rect& rect = glyph.second.rect;
    WriteRect(rect, out, cream, 1, burntOrange);

    if (writeKey)
    {
        std::string key = glyph.first.ToString();
        out << indent << R"(<text x=")" << rect.x + rect.w / 3 << R"(" y=")" << rect.y + rect.h / 2 << '"'
            << R"( font-family="Verdana" font-size="6" fill="black">)" << key << R"(</text>)" << "\n";
    }
}