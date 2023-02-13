#pragma once

#include "FontKey.h"

#include <vector>
#include <string>
#include <sstream>

struct GlyphDataForAtlas
{
    int fontId;
    std::vector<int> glyphIds;

    GlyphDataForAtlas(std::string serialized)
    {
        std::istringstream in {serialized};
        in >> fontId;
        int glyphId;
        while (in >> glyphId)
        {
            glyphIds.push_back(glyphId);
        }
    }
};