#pragma once

#include "Tuple.h"
#include <string>

class Rect
{
public:
    ushort x{};
    ushort y{};

    ushort w{};
    ushort h{};

    Rect();
    Rect(ushort xArg, ushort yArg, ushort wArg, ushort hArg);
    explicit Rect(const std::string&);

    [[nodiscard]] int GetArea() const { return w * h;}
    void Update(ushort xArg, ushort yArg, ushort wArg, ushort hArg);

    [[nodiscard]] std::string ToString() const;
};
