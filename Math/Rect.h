#pragma once

#include "Tuple.h"

extern "C" typedef struct CRect
{
    ushort x;
    ushort y;
    ushort w;
    ushort h;
} CRect;

#include <string>

struct Rect : CRect
{
    Rect();

    Rect(ushort xArg, ushort yArg, ushort wArg, ushort hArg);

    explicit Rect(const std::string &);

    [[nodiscard]] int GetArea() const
    { return w * h; }

    void Update(ushort xArg, ushort yArg, ushort wArg, ushort hArg);

    [[nodiscard]] std::string ToString() const;
};
