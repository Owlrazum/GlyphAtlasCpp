#include "SimdUtfWrapper.h"

bool SimdUtfWrapper::IsValidUtf8(const std::string &text)
{
    return simdutf::validate_utf8(text.c_str(), text.length());
}

bool SimdUtfWrapper::IsValidUtf8(const char *text)
{
    return simdutf::validate_utf8(text, strlen(text));
}
