#pragma once

#include <ostream>
typedef unsigned short ushort;

struct Pair
{
    Pair(ushort xArg, ushort yArg) : x(xArg), y(yArg) {}
    ushort x;
    ushort y;
    friend std::ostream& operator<<(std::ostream& out, Pair& pair)
    {
        return out << pair.x << " " << pair.y;
    }
};

struct Triple
{
    ushort x;
    ushort y;
    ushort z;
};

struct Quad
{
    ushort x;
    ushort y;
    ushort z;
    ushort w;
};