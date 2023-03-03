#pragma once

#include "simdutf.h"

class SimdUtfWrapper
{
public:
    bool IsValidUtf8(std::string text)
    {
        return simdutf::validate_utf8(text.c_str(), text.length());
    }

    bool IsValidUtf8(const char* text)
    {
        return simdutf::validate_utf8(text, strlen(text));
    }
};