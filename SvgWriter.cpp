//
// Created by Abai on 08.02.2023.
//

#include "SvgWriter.h"

SvgWriter::SvgWriter(std::string path, std::pair<ushort, ushort> dims)
 :out(path)
{
    if (out.is_open())
    {
        std::cout << "Created a file at " << path << std::endl;
    }

    std::string svgDims = indent + "width=\"" + std::to_string(dims.first) +
            "\" height=\"" + std::to_string(dims.second) + "\">\n\n";

    out << svgOpen << svgDims << svgBack;
}

SvgWriter::~SvgWriter()
{
    out << svgClose;
    out.close();
}

void SvgWriter::WriteRect(Rect rect)
{


}
