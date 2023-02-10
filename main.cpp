#include <iostream>

#include "Slack.h"
#include "GlyphAtlasUtils.h"
#include "GlyphAtlas.h"

extern "C" DLLEXPORT int Test()
{
    Rect rect;
    return 42;
}

std::vector<Rect> Generation(const std::string path, int count)
{
    auto rects = GenerateRects(count, {8, 64, 8, 64});
    WriteRects(path, rects);
    return rects;
}

std::vector<Rect> Reading(const std::string path)
{
    auto rects = ReadRects(path);
    return rects;
}

int main()
{
    const std::string path = "../rects.txt";

//    auto rects = Generation(path, 50);
    auto rects = Reading(path);

    const std::vector<ushort> heightDelimiters {8, 16, 24, 32, 40, 48, 56, 64};
    const std::vector<ushort> widthDelimiters {8, 16, 24, 32, 40, 48, 56, 64};

    GlyphAtlas atlas {heightDelimiters, widthDelimiters};
    atlas.Update(rects);
    WriteGlyphAtlas(atlas);
    std::cout << "completed" << std::endl;
    return 0;
}