//
// Created by Abai on 08.02.2023.
//

#ifndef GLYPHATLAS_SVGWRITER_H
#define GLYPHATLAS_SVGWRITER_H

#include "Rect.h"

#ifndef STRING_H
#define STRING_H
#include <string>
#endif

#ifndef COLORS_H
#define COLORS_H
#include "Colors.h"
#endif

#include <fstream>
#include <iostream>

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

#endif //GLYPHATLAS_SVGWRITER_H
