#include "FileUtils.h"

#include <fstream>
#include <random>

std::string GetFontPath(int fontId)
{
    std::string path = "../TestData/font_.txt";
    path.insert(17, std::to_string(fontId));
    return path;
}

std::string GetDataPath(int testNumber)
{
    std::string path = "../TestData/test_.txt";
    path.insert(17, std::to_string(testNumber));
    return path;
}

std::string GetTexturePath(int textureId)
{
    std::string path = "../ResultTextures/texture_.svg";
    path.insert(26, std::to_string(textureId));
    return path;
}

void WriteRects(const std::string &path, const std::vector<Rect> &rects)
{
    std::ofstream out{path};
    for (Rect rect: rects)
    {
        out << rect.ToString();
    }
}

void GenerateAndWriteGlyphKeys(
        const std::string &path,
        int fontCount,
        const std::vector<int> &glyphCounts)
{
    std::ofstream out{path};

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
            out << " " << glyphs[g];
        }
        out << std::endl;
    }
}

std::vector<GlyphKey> ReadGlyphKeys(const std::string &path)
{
    std::ifstream in{path};
    std::string line;
    std::vector<GlyphKey> data;
    while (std::getline(in, line))
    {
        std::istringstream ss{line};
        ushort fontId;
        ss >> fontId;
        ushort glyphId;
        while (ss >> glyphId)
        {
            data.push_back({fontId, glyphId});
        }
    }

    return data;
}

std::vector<std::vector<GlyphKey>> ReadGlyphKeysByLine(const std::string &path)
{
    std::ifstream in{path};
    std::string line;
    std::vector<std::vector<GlyphKey>> data;
    while (std::getline(in, line))
    {
        std::istringstream ss{line};
        ushort fontId;
        ss >> fontId;
        ushort glyphId;
        std::vector<GlyphKey> lineGlyphs;
        while (ss >> glyphId)
        {
            lineGlyphs.push_back({fontId, glyphId});
        }
        data.push_back(lineGlyphs);
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
        std::ifstream in{GetFontPath(key.fontId)};
        int lineCounter = 0;
        while (std::getline(in, line))
        {
            if (lineCounter == key.glyphId)
            {
                break;
            }
            lineCounter++;
        }
        Glyph glyph (line);
        glyphs.emplace_back(key, glyph);
    }

    return glyphs;
}
