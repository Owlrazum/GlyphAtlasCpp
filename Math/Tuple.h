#pragma once

typedef unsigned short ushort;

struct Pair
{
    Pair(ushort xArg, ushort yArg) : x(xArg), y(yArg) {}
    ushort x;
    ushort y;
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