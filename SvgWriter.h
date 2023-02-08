//
// Created by Abai on 08.02.2023.
//

#ifndef GLYPHATLAS_SVGWRITER_H
#define GLYPHATLAS_SVGWRITER_H

#include "GlyphAtlas/Rect.h"
#include <fstream>
#include <string>
#include <iostream>

class SvgWriter
{
public:
    SvgWriter(std::string path, std::pair<ushort, ushort> dims);

    ~SvgWriter();

    void WriteRect(Rect rect);

private:
    std::ofstream out;

    inline static const std::string indent = "    ";

    inline static const std::string svgOpen = "<svg version=\"1.1\"\n" +
                                              indent + "xmlns=\"http://www.w3.org/2000/svg\"\n";
    inline static const std::string svgBack = indent + "<rect width=\"100%\" height=\"100%\" fill=\"black\" />"
                                                       "\n";
    inline static const std::string svgClose = "\n</svg>";
};

#endif //GLYPHATLAS_SVGWRITER_H
