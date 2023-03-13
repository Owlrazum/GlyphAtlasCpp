#include "FileUtils.h"

#include <fstream>
#include <exception>
#include <random>

#include "simdutf.h"

std::string GetFontPath(machine fontId)
{
    switch(fontId)
    {
        case 0: return "/Users/Abai/Desktop/Evolve/GlyphAtlas/Fonts/Times_New_Roman.ttf";
        case 1: return "/Users/Abai/Desktop/Evolve/GlyphAtlas/Fonts/Lato-Regular.ttf";
        case 2: return "/Users/Abai/Desktop/Evolve/GlyphAtlas/Fonts/Lato-Thin.ttf";
        case 3: return "/Users/Abai/Desktop/Evolve/GlyphAtlas/Fonts/Lato-Black.ttf";
        case 4: return "/Users/Abai/Desktop/Evolve/GlyphAtlas/Fonts/Lato-LightItalic.ttf";
        default: throw std::out_of_range("Unknown fontIndex" + std::to_string(fontId));
    }
}
std::string GetTestGlyphKeysPath(machine testNumber)
{
    std::string path = "/Users/Abai/Desktop/Evolve/GlyphAtlas/TestData/test_.txt";
    path.insert(52, std::to_string(testNumber));
    return path;
}
std::string GetSvgTexturePath(machine textureId)
{
    std::string path = "/Users/Abai/Desktop/Evolve/GlyphAtlas/ResultTextures/texture_.svg";
    path.insert(61, std::to_string(textureId));
    return path;
}

void WriteTestGlyphKeys(
        std::ofstream &out,
        machine fontCount,
        machine keysCountInPass)
{
    std::random_device rd; // obtain a random number from hardware
    std::default_random_engine gen(rd()); // seed the generator
    auto fontGen = std::bind(std::uniform_int_distribution<> (0, static_cast<int>(fontCount) - 1), gen);
    auto glyphGen = std::bind(std::uniform_int_distribution<> ('a', 'z'), gen);
    auto boolGen = std::bind(std::uniform_int_distribution<> (0, 1), gen);

    for (machine i = 0; i < keysCountInPass; i++)
    {
        out << fontGen() << " ";
        if (boolGen())
        {
            out << (char)glyphGen();
        }
        else
        {
            out << (char)toupper(glyphGen());
        }

        if (i != keysCountInPass - 1)
        {
            out << " ";
        }
    }
}

std::vector<std::vector<std::pair<GlyphKey, Glyph>>> ReadGlyphKeysByLine(const std::string &path)
{
    std::ifstream in{path};
    std::string line;
    std::vector<std::vector<std::pair<GlyphKey, Glyph>>> data;
    uint8 fontIndex;
    char32_t glyphCharacter;
    char buffer[4];
    while (std::getline(in, line))
    {
        std::basic_istringstream<char> ss{line};
        std::vector<std::pair<GlyphKey, Glyph>> lineKeys;
        while(ss >> fontIndex >> buffer)
        {
            bool result = simdutf::validate_utf8(buffer, 4);
            if (!result)
            {
                throw std::out_of_range("not valid utf8");
            }
            auto conversionResult = simdutf::convert_utf8_to_utf32_with_errors(buffer, 1, &glyphCharacter);
            if (conversionResult.error != simdutf::SUCCESS)
            {
                throw std::out_of_range("error with conversion to utf32 from utf8");
            }
            fontIndex -= '0';
            GlyphKey key {fontIndex, glyphCharacter};
            Glyph glyph;
            lineKeys.emplace_back(key, glyph);
        }
        data.push_back(lineKeys);
    }

    return data;
}

// obsoletes
//void WriteTestFontRects(const std::string &path, const std::vector<Rect> &rects)
//{
//    std::ofstream out{path};
//    for (machine i = 0; i < rects.size(); i++)
//    {
//        out << rects[i].ToString();
//        if (i != rects.size() - 1)
//        {
//            out << "\n";
//        }
//    }
//}

//std::string GetTestFontPath(machine fontId)
//{
//    std::string path = "/Users/Abai/Desktop/Evolve/GlyphAtlas/TestData/font_.txt";
//    path.insert(52, std::to_string(fontId));
//    return path;
//}


//void ReadGlyphRects(std::vector<std::pair<GlyphKey, Glyph>> &glyphs)
//{
//    std::string line;
//    for (auto pair : glyphs)
//    {
//        auto key = pair.first;
//        std::ifstream in{GetTestFontPath(key.fontIndex)};
//        machine lineCounter = 0;
//        while (std::getline(in, line))
//        {
//            if (lineCounter == key.glyphIndex)
//            {
//                break;
//            }
//            lineCounter++;
//        }
//        auto &glyph = pair.second;
//        glyph.Update(line);
//        assert(glyph.rect.w > 0 && glyph.rect.h > 0);
//    }
//}
