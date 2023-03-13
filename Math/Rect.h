#pragma once

#include "PrimitiveTypes.h"

extern "C" typedef struct CRect
{
    bool operator<(const CRect &rhs) const;

    bool operator>(const CRect &rhs) const;

    bool operator<=(const CRect &rhs) const;

    bool operator>=(const CRect &rhs) const;

    uint16 x;
    uint16 y;
    uint16 w;
    uint16 h;
} CRect;

#include <string>

struct Rect : CRect
{
    Rect();

    Rect(uint16 xArg, uint16 yArg, uint16 wArg, uint16 hArg);

    explicit Rect(const std::string &);

    [[nodiscard]] uint32 GetArea() const
    { return w * h; }

    [[nodiscard]] std::string ToString() const;
};
