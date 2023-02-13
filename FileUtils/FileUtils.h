#pragma once

#include "Math/Rect.h"
#include "GlyphDataForAtlas.h"

#include <vector>
#include <fstream>
#include <random>
#include <algorithm>

std::string GetFontPath(int fontId)
{
    std::string path = "../TestData/font_.txt";
    path.insert(17, std::to_string(fontId));
    return path;
}

std::string GetDataPath(int dataIndex)
{
    std::string path = "../TestData/test_.txt";
    path.insert(17, std::to_string(dataIndex));
    return path;
}

static std::vector<Rect> ReadRects(const std::string path)
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

static void GenerateAndWriteGlyphDataForAtlas(const std::string path, int fontCount, const std::vector<int> glyphCounts)
{
    std::ofstream out {path};

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> rndGenFontCount(0, fontCount);

    int rndFontCount = rndGenFontCount(gen);
    std::vector<int> fonts;
    for (int i = 0; i < fontCount; i++)
    {
        fonts.push_back(i);
    }
    std::shuffle(fonts.begin(), fonts.end(), gen);

    for (int i = 0; i < rndFontCount; i++)
    {
        int rndFont = fonts[i];

        std::vector<int> glyphs;
        for (int g = 0; g < glyphCounts[rndFont]; g++)
        {
            glyphs.push_back(g);
        }

        std::shuffle(glyphs.begin(), glyphs.end(), gen);
        std::uniform_int_distribution<> rndGenGlyphCount(0, glyphCounts[i]);
        int rndGlyphCount = rndGenGlyphCount(gen);

        out << rndFont;
        for (int g = 0; g < rndGlyphCount; g++)
        {
             out  << " " << glyphs[g];
        }
        out << std::endl;
    }
}

static std::vector<GlyphDataForAtlas> ReadGlyphDataForAtlas(const std::string path)
{
    std::ifstream in {path};
    std::string line;
    std::vector<GlyphDataForAtlas> data;
    while(std::getline(in, line))
    {
        GlyphDataForAtlas fontData {line}; // maybe throw an exception if not okay
        data.push_back(fontData);
    }

    return data;
}