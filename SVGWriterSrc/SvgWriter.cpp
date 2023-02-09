//
// Created by Abai on 08.02.2023.
//

#include "SvgWriter.h"

SvgWriter::SvgWriter(std::string path, Pair dims, std::string backColor)
 :out(path)
{
    if (out.is_open())
    {
        std::cout << "Opened a file at " << path << std::endl;
    }

    const std::string svgDims = indent + "width=\"" + std::to_string(dims.x) +
            "\" height=\"" + std::to_string(dims.y) + "\">\n\n";

    const std::string svgBack = indent + "<rect width=\"100%\" height=\"100%\" fill=\"" + backColor + "\" />"
                                                       "\n";

    out << svgOpen << svgDims << svgBack;
}

SvgWriter::~SvgWriter()
{
    out << svgClose;
    out.close();
}

void SvgWriter::WriteRect(Rect rect, std::string color)
{
    std::string svgRect = indent + "<rect x=\"" + std::to_string(rect.x) +
            "\" y=\"" + std::to_string(rect.y) +
            "\" width=\"" + std::to_string(rect.w) +
            "\" height=\""+ std::to_string(rect.h) +
            "\" fill=\"" + color + "\" /> \n";
    out << svgRect;
}

void SvgWriter::WriteRect(Rect rect, std::string fillColor, ushort borderWidthHalf, std::string borderColor)
{
    std::string svgRect = indent + "<rect x=\"" + std::to_string(rect.x + borderWidthHalf) +
                          "\" y=\"" + std::to_string(rect.y + borderWidthHalf) +
                          "\" width=\"" + std::to_string(rect.w - borderWidthHalf * 2) +
                          "\" height=\""+ std::to_string(rect.h - borderWidthHalf * 2) +
                          "\" fill=\"" + fillColor +
                          "\" stroke-width=\"" + std::to_string(borderWidthHalf * 2) +
                          "\" stroke=\"" + borderColor + "\"/> \n";
    out << svgRect;
}
