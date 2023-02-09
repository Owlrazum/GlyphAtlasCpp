#include <iostream>
#include <format>

#include "Slack.h"
#include "SvgWriter.h"

extern "C" DLLEXPORT int Test()
{
    Rect rect;
    return 42;
}

int main()
{
    Pair dims {400, 400};
    SvgWriter writer("../Visual.svg", dims, whiteBack);
    Rect rect {0, 0, 90, 90};
    while (rect.y < dims.y)
    {
        while (rect.x < dims.x)
        {
            writer.WriteRect(rect, cream, 1, burntOrange);
            rect.x += 103; // hard for me to justify why exactly "103"
        }
        rect.y += 103;
        rect.x = 0;
    }

    writer.BlankLine();
    writer.WriteRect(Rect {125, 125, 150, 150}, whiteBack);

    Pair innerConstraints {250, 250};
    rect.Update(150, 150, 50, 50);
    while(rect.y < innerConstraints.y)
    {
        while(rect.x < innerConstraints.x)
        {
            writer.WriteRect(rect, burntOrange, 1, cream);
            rect.x += 50;
        }
        rect.y += 50;
        rect.x = 150;
    }

    std::cout << "completed" << std::endl;
    return 0;
}