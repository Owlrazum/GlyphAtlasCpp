#include <iostream>
#include <format>

#include "GlyphAtlas/Rect.h"
#include "GlyphAtlas/Slack.h"
#include "SvgWriter.h"

extern "C" DLLEXPORT int Test()
{
    Rect rect;
    return 42;
}

int main()
{
    SvgWriter writer("../Visual.svg", std::make_pair(100, 400));
    std::cout << "completed" << std::endl;
    return 0;
}