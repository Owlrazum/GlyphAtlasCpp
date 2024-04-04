#include "FileUtils.h"

#include <fstream>
#include <random>
#include <array>
#include <functional>
#include <cstring>

#include "simdutf.h"

#include <iostream>

std::string GetFontPath(machine fontId)
{
    std::string prefix {PROJECT_SOURCE_DIR};
    switch(fontId)
    {
        case 0: return prefix + "/Fonts/IndividigitalThin-doM7.ttf";
        case 1: return prefix + "/Fonts/Lato-Thin.ttf";
        case 2: return prefix + "/Fonts/Times_New_Roman.ttf";
        case 3: return prefix + "/Fonts/Lato-Black.ttf";
        case 4: return prefix + "/Fonts/Lato-LightItalic.ttf";
        default: throw std::out_of_range("Unknown fontIndex" + std::to_string(fontId));
    }
}
std::string GetTestGlyphKeysPath(machine testNumber)
{
    std::string prefix {PROJECT_SOURCE_DIR};
    std::string path = prefix + "/TestData/test_.txt";
    path.insert(path.length() - 4, std::to_string(testNumber));
    std::cout<<PROJECT_SOURCE_DIR;
    return path;
}

void WriteGlyphTestData(
        std::ofstream &out,
        machine fontCount,
        machine keysCountInPass)
{
    std::random_device rd; // obtain a random number from hardware
    std::default_random_engine gen(rd()); // seed the generator
    auto fontIdGen = std::bind(std::uniform_int_distribution<> (0, static_cast<int>(fontCount) - 1), gen);
    auto fontSizeGen = std::bind(std::uniform_int_distribution<> (20, 32), gen);
    auto glyphGen = std::bind(std::uniform_int_distribution<> ('a', 'z'), gen);
    auto boolGen = std::bind(std::uniform_int_distribution<> (0, 1), gen);

    for (machine i = 0; i < keysCountInPass; i++)
    {
        out << 2 << " " << fontSizeGen() << " ";
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

uint32 ParseUInt32(const char (&buf)[4])
{
    // This prevents buf[i] from being promoted to a signed int.
    uint32 u0 = buf[0], u1 = buf[1], u2 = buf[2], u3 = buf[3];
    uint32 uval = u0 | (u1 << 8) | (u2 << 16) | (u3 << 24);
    return uval;
}

std::vector<std::vector<std::pair<FontKey, GlyphKey>>> ReadGlyphTestData(const std::string &path)
{
    std::ifstream in{path};
    std::string line;
    std::vector<std::vector<std::pair<FontKey, GlyphKey>>> data;
    uint8 fontIndex;
    int32 fontSize;

    // todo check whether glyphCharacter should be char32_t or uint32
    char32_t glyphCharacter;
    char buffer[4];
    memset(buffer, '\0', 4);
    while (std::getline(in, line))
    {
        std::istringstream ss (line);
        std::vector<std::pair<FontKey, GlyphKey>> lineKeys;
        while(ss >> fontIndex >> fontSize >> buffer)
        {
            bool result = simdutf::validate_utf8(buffer, 4);
            if (!result)
            {
                std::string msg = "not valid utf8 ";
                msg.append(buffer);
                throw std::out_of_range(msg);
            }
            auto conversionResult = simdutf::convert_utf8_to_utf32_with_errors(buffer, 1, &glyphCharacter);
            if (conversionResult.error != simdutf::SUCCESS)
            {
                throw std::out_of_range("error with conversion to utf32 from utf8");
            }
            fontIndex -= '0';
            uint8 defaultFaceIndex = 0;

            FontKey fontKey {fontIndex, defaultFaceIndex, fontSize * 64};
            glyphCharacter = ParseUInt32(buffer);
            GlyphKey glyphKey {0, glyphCharacter}; // the GlyphKey.fontId will be populated by FreeTypeWrapper
            lineKeys.emplace_back(fontKey, glyphKey);
        }
        data.push_back(lineKeys);
    }

    return data;
}