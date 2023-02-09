#include <iostream>
#include <format>

#include "Slack.h"
#include "GlyphAtlasUtils.h"
#include "GlyphAtlas.h"

extern "C" DLLEXPORT int Test()
{
    Rect rect;
    return 42;
}

vector<Rect> Generation(const std::string path, int count)
{
    auto rects = GenerateRects(count, {8, 64, 8, 64});
    WriteRects(path, rects);
    return rects;
}

vector<Rect> Reading(const std::string path)
{
    auto rects = ReadRects(path);
    return rects;
}

int main()
{
    const std::string path = "../rects.txt";

    auto rects = Generation(path, 50);
//    auto rects = Reading(path);

    GlyphAtlas atlas;
    atlas.Update(rects);
    WriteGlyphAtlas(atlas);
    std::cout << "completed" << std::endl;
    return 0;
}