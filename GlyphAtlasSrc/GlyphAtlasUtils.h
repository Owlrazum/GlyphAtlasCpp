//
// Created by Abai on 09.02.2023.
//
#include <random>

#include "GlyphAtlas.h"
#include "SvgWriter.h"
#include "Colors.h"

static std::vector<Rect> ReadRects(const std::string path, const size_t maxRectsCount = 50)
{
    std::ifstream in {path};
    std::string line;
    std::vector<Rect> rects;
    while(std::getline(in, line))
    {
        Rect rect {line}; // maybe throw an exception if not okay
        rects.push_back(rect);
    }

    return rects;
}

static void WriteRects(const std::string path, const std::vector<Rect> rects)
{
    std::ofstream out {path};
    for (Rect rect : rects)
    {
        out << rect.ToString();
    }
}

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

static void WriteGlyphAtlas(const GlyphAtlas& atlas)
{
    std::cout << "writing to ../Visual.svg";
    SvgWriter writer("../Visual.svg", atlas.GetDims(), whiteBack);
    auto rects = atlas.GetRects();
    for (Rect rect : rects)
    {
        writer.WriteRect(rect, cream, 1, burntOrange);
    }
}
