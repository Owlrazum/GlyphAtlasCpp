#pragma once

#include "Rect.h"
#include <string>
#include <sstream>

struct Glyph
{
    Glyph()
    {
        textureId = 0;
    }

    ushort textureId;
    Rect rect;

    Glyph(ushort textureIdArg, Rect rectArg) : textureId(textureIdArg), rect(rectArg) {}

    explicit Glyph(const std::string& serialized)
    {
        std::istringstream in {serialized};
        std::string buf;
        in >> buf >> rect.x >> rect.y >> buf >> rect.w >> rect.h >> buf;
        textureId = 0;
    }

    [[nodiscard]] std::string ToString() const
    {
        return "Glyph\n textureId: " + std::to_string(textureId) + "\n" + rect.ToString();
    }
};