#pragma once

#include "simdutf.h"

class SimdUtfWrapper
{
public:
    static bool IsValidUtf8(const std::string& text);
    static bool IsValidUtf8(const char* text);
};