#pragma once

#include "Tuple.h"
#include <string>

class Rect
{
public:
    ushort x;
    ushort y;

    ushort w;
    ushort h;

    Rect();
    Rect(ushort xArg, ushort yArg, ushort wArg, ushort hArg);
    Rect(std::string);

    int GetArea() { return w * h;}
    void Update(ushort x, ushort y, ushort w, ushort h);

    std::string ToString() const;
};
