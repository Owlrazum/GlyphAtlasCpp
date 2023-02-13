#include <iostream>

#include "Slack.h"
#include "GlyphAtlasUtils.h"
#include "FileUtils.h"
#include "GlyphAtlas.h"

#include <vector>
#include <iostream>

extern "C" DLLEXPORT int Test()
{
    Rect rect;
    return 42;
}

// Generates files in TestData/ using the following conventions:
// 1. font_x.txt mimics font by listing all rects one by one on each line
// 2. test.x.txt lists lines each containing font index leading to font_x.txt file,
// then a list of glyph indices inside that font leading to Rect
void GenerateTestCases(int fontCount, std::vector<int> glyphCounts, int testCount)
{
    for (int i = 0; i < fontCount; i++)
    {
        auto rects = GenerateRects(glyphCounts[i], {8, 64, 8, 64});
        WriteRects(GetFontPath(i), rects);
    }

    for (int i = 0; i < testCount; i++)
    {
        GenerateAndWriteGlyphDataForAtlas(GetDataPath(i), fontCount, glyphCounts);
    }
}

int main()
{
    const int FONT_COUNT = 10;
    const std::vector<int> glyphCounts {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    GenerateTestCases(FONT_COUNT, glyphCounts, 2);
//
//    const std::vector<ushort> heightDelimiters {8, 16, 24, 32, 40, 48, 56, 64};
//    const std::vector<ushort> widthDelimiters {8, 16, 24, 32, 40, 48, 56, 64};
//    GlyphAtlas atlas {heightDelimiters, widthDelimiters};
//
//    std::vector<std::vector<Rect>> rectsByFont;
//    for (int i = 0; i < FONT_COUNT; i++)
//    {
//        auto rects = ReadRects(GetFontPath(i));
//        rectsByFont.push_back(rects);
//    }
//    atlas.Update(rectsByFont);

//    WriteGlyphAtlas(atlas);
//    std::cout << "completed" << std::endl;
    return 0;
}