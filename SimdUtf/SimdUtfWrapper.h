#pragma once

#include "simdutf.h"

class SimdUtfWrapper
{
public:
    static bool IsValidUtf8(const std::string& text)
    {
        return simdutf::validate_utf8(text.c_str(), text.length());
    }

    static bool IsValidUtf8(const char* text)
    {
        return simdutf::validate_utf8(text, strlen(text));
    }
};