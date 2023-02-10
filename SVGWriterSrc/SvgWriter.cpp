#include "SvgWriter.h"

#include <sstream>
#include <iostream>
#include <string>

SvgWriter::SvgWriter(std::string path, Pair dims, std::string backColor)
 :out(path)
{
    std::stringstream ss;

    // texture dimensions
    ss << indent << "width=\"" << dims.x << "\" height=\"" << dims.y << "\">\n\n";
    // background
    ss << indent << "<rect width=\"100%\" height=\"100%\" fill=\"" << backColor << "\" />\n";
    out << svgOpen << ss.str();
}

SvgWriter::~SvgWriter()
{
    out << svgClose;
    out.close();
}

void SvgWriter::WriteRect(Rect rect, std::string color)
{
    std::stringstream ss;

    ss << indent << "<rect x=\"" << rect.x << "\" y=\"" << rect.y
       << "\" width=\"" << rect.w << "\" height=\"" << rect.h
       << "\" fill=\"" << color << "\" /> \n";
    out << ss.str();
}

void SvgWriter::WriteRect(Rect rect, std::string fillColor, ushort borderWidthHalf, std::string borderColor)
{
    std::stringstream ss;

    ss << indent << "<rect x=\"" << rect.x + borderWidthHalf << "\" y=\"" << rect.y + borderWidthHalf
       << "\" width=\"" << rect.w - borderWidthHalf * 2 << "\" height=\"" << rect.h - borderWidthHalf * 2
       << "\" fill=\"" << fillColor
       << "\" stroke-width=\"" << borderWidthHalf * 2
       << "\" stroke=\"" << borderColor << "\"/> \n";
    out << ss.str();
}
