#pragma once

#include "Math/Rect.h"
#include "Colors.h"

#include <fstream>
#include <string>

class SvgWriter
{
public:
    SvgWriter(std::string path, Pair dims, std::string backColor);

    ~SvgWriter();

    void BlankLine()
    {
        out << "\n";
    }

    void WriteRect(Rect rect, std::string color);
    void WriteRect(Rect rect, std::string fillColor, ushort borderWidthHalf, std::string borderColor);

private:
    std::ofstream out;

    inline static const std::string indent = "    ";

    inline static const std::string svgOpen = "<svg version=\"1.1\"\n" +
                                              indent + "xmlns=\"http://www.w3.org/2000/svg\"\n";

    inline static const std::string svgClose = "\n</svg>";
};