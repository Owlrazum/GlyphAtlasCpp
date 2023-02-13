#pragma once

#include "GlyphAtlas.h"
#include "SvgWriter.h"
#include "Colors.h"

#include <random>

static std::vector<Rect> GenerateRects(int count, Quad minMax)
{
    std::vector<Rect> rects; // toask: should I preallocate here?
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> width(minMax.x, minMax.y); // define the range
    std::uniform_int_distribution<> height(minMax.z, minMax.w); // define the range
    for (int i = 0; i < count; i++)
    {
        Rect rect { 0, 0,
                    static_cast<ushort>(width(gen)),
                    static_cast<ushort>(height(gen))};
        rects.push_back(rect);
    }
    return rects;
}

static std::vector<int> GenerateGlyphIds(int count)
{
    std::vector<int> data;
    for (int i = 0; i < count; i++)
    {
        data.push_back(i);
    }

    return data;
}

static void WriteGlyphAtlas(const GlyphAtlas& atlas)
{
    std::cout << "writing to ../Visual.svg\n";
    SvgWriter writer("../Visual.svg", atlas.GetTextureDims(0), whiteBack);
    auto rects = atlas.GetGlyphs();
    for (Glyph glyph : rects)
    {
        writer.WriteRect(glyph.rect, cream, 1, burntOrange);
    }
}
