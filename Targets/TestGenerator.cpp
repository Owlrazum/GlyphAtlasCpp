#include <iostream>

#include "GlyphAtlasUtils.h"
#include "FileUtils.h"
#include "GlyphAtlas.h"

#include <vector>
#include <fstream>

void GenerateFonts(int fontCount, int glyphCountInFont);

// Generates files in TestData/ using the following conventions:
// 1. font_x.txt mimics font by listing all rects one by one on each line
// 2. test.x.txt lists lines each containing font index leading to font_x.txt file,
// then a list of glyph indices inside that font leading to Rect
void GenerateTestCase(
        int testNumber,
        int passCount,
        int fontCount,
        int glyphCountInFont,
        int keysCountInPass);

void Test(int caseNumber, GlyphAtlas &atlas);

int main()
{
//    GenerateFonts(3, 10);
//    GenerateTestCase(0, 1, 3, 10, 3);

    const std::vector<ushort> shelfDelimiters{64, 96, 128};
    const std::vector<ushort> slotDelimiters{64, 84, 100, 128};
    GlyphAtlas atlas{shelfDelimiters, slotDelimiters, {1024, 1024}};

    Test(0, atlas);

//    SvgWriter writer (whiteBack);
//    writer.WriteAtlas(atlas);
    return 0;
}

void GenerateFonts(int fontCount, int glyphCountInFont)
{
    for (int i = 0; i < fontCount; i++)
    {
        auto rects = GenerateRects(glyphCountInFont, {64, 128, 64, 128});
        WriteTestFontRects(GetTestFontPath(i), rects);
    }
}

void GenerateTestCase(
        int testNumber,
        int passCount,
        int fontCount,
        int glyphCountInFont,
        int keysCountInPass)
{
    std::ofstream out {GetTestGlyphKeysPath(testNumber)};
    for (int i = 0; i < passCount; i++)
    {
        WriteTestGlyphKeys(out, fontCount, glyphCountInFont, keysCountInPass);
        if (i != passCount - 1)
        {
            out << "\n";
        }
    }
}

void Test(int caseNumber, GlyphAtlas &atlas)
{
    auto testCase = ReadGlyphKeysByLine(GetTestGlyphKeysPath(caseNumber));
    for (const auto & keys : testCase)
    {
        atlas.Update(keys);
    }

    std::cout << "completed test case " << caseNumber << std::endl;
}