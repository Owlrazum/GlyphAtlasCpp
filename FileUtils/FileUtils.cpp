#include "FileUtils.h"

#include <fstream>
#include <random>

std::string GetTestFontPath(int fontId)
{
    std::string path = "/Users/Abai/Desktop/Evolve/GlyphAtlas/TestData/font_.txt";
    path.insert(52, std::to_string(fontId));
    return path;
}

std::string GetTestGlyphKeysPath(int testNumber)
{
    std::string path = "/Users/Abai/Desktop/Evolve/GlyphAtlas/TestData/test_.txt";
    path.insert(52, std::to_string(testNumber));
    return path;
}

std::string GetSvgTexturePath(int textureId)
{
    std::string path = "/Users/Abai/Desktop/Evolve/GlyphAtlas/ResultTextures/texture_.svg";
    path.insert(61, std::to_string(textureId));
    return path;
}

void WriteTestFontRects(const std::string &path, const std::vector<Rect> &rects)
{
    std::ofstream out{path};
    for (int i = 0; i < rects.size(); i++)
    {
        out << rects[i].ToString();
        if (i != rects.size() - 1)
        {
            out << "\n";
        }
    }
}

void WriteTestGlyphKeys(
        std::ofstream &out,
        int fontCount,
        int glyphCountInFont,
        int keysCountInPass)
{
    std::random_device rd; // obtain a random number from hardware
    std::default_random_engine gen(rd()); // seed the generator
    std::uniform_int_distribution<> fontDistr(0, fontCount - 1);
    std::uniform_int_distribution<> glyphDistr(0, glyphCountInFont - 1);
    auto fontGen = std::bind(fontDistr, gen);
    auto glyphGen = std::bind(glyphDistr, gen);

    for (int i = 0; i < keysCountInPass; i++)
    {
        out << fontGen() << " " << glyphGen();
        if (i != keysCountInPass - 1)
        {
            out << " ";
        }
    }
}

std::vector<std::vector<GlyphKey>> ReadGlyphKeysByLine(const std::string &path)
{
    std::ifstream in{path};
    std::string line;
    std::vector<std::vector<GlyphKey>> data;
    unsigned char fontId;
    ushort glyphId;
    while (std::getline(in, line))
    {
        std::istringstream ss{line};
        std::vector<GlyphKey> lineKeys;
        while(ss >> fontId >> glyphId)
        {
            lineKeys.push_back({fontId, glyphId});
        }
        data.push_back(lineKeys);
    }

    return data;
}

std::vector<std::pair<GlyphKey, Glyph>> ReadGlyphs(const std::vector<GlyphKey>& keys)
{
    std::vector<std::pair<GlyphKey, Glyph>> glyphs;
    glyphs.reserve(keys.size());
    std::string line;
    for (auto key : keys)
    {
        std::ifstream in{GetTestFontPath(key.fontIndex)};
        int lineCounter = 0;
        while (std::getline(in, line))
        {
            if (lineCounter == key.glyphIndex)
            {
                break;
            }
            lineCounter++;
        }
        Glyph glyph (line);
        assert(glyph.rect.w > 0 && glyph.rect.h > 0);
        glyphs.emplace_back(key, glyph);
    }

    return glyphs;
}

std::string GetFontPath(int fontId)
{
    switch(fontId)
    {
        case 0: return "/Users/Abai/Desktop/Evolve/GlyphAtlas/Fonts/Lato-Regular.ttf";
        case 1: return "/Users/Abai/Desktop/Evolve/GlyphAtlas/Fonts/Times_New_Roman.ttf";
        case 2: return "/Users/Abai/Desktop/Evolve/GlyphAtlas/Fonts/Lato-Thin.ttf";
        case 3: return "/Users/Abai/Desktop/Evolve/GlyphAtlas/Fonts/Lato-Black.ttf";
        default: throw std::out_of_range("Unknown fontIndex" + std::to_string(fontId));
    }
}


/*
 * std::vector<int> fonts (fontCount);
    std::iota(fonts.begin(), fonts.end(), fontCount);

    for (int i = 0; i < fontCount; i++)
    {
        int font = fonts[i];

        std::vector<int> glyphs (glyphCounts[font]);
        std::iota(glyphs.begin(), glyphs.end(), glyphCounts[font]);
        std::shuffle(glyphs.begin(), glyphs.end(), gen);

        std::uniform_int_distribution<> rndGenGlyphCount(0, glyphCounts[i]);
        int rndGlyphCount = rndGenGlyphCount(gen);

        out << font;
        for (int g = 0; g < rndGlyphCount; g++)
        {
            out << " " << glyphs[g];
        }
        out << std::endl;
    }
 */