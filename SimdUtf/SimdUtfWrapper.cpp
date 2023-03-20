#include "SimdUtfWrapper.h"

    void SerializeUInt32(char (&buf)[4], uint32 val)
{
    uint32 uval = val;
    buf[0] = uval;
    buf[1] = uval >> 8;
    buf[2] = uval >> 16;
    buf[3] = uval >> 24;
}

void SimdUtfWrapper::ConvertToUtf32(GlyphKey &glyphKey)
{
    char buffer[4];
    memset(buffer, '\0', 4);
    SerializeUInt32(buffer, glyphKey.character);
    char32_t glyphCharacter;

    bool result = simdutf::validate_utf8(buffer, 4);
    if (!result)
    {
        std::string msg = "not valid utf8 ";
        msg.append(buffer);
        throw std::out_of_range(msg);
    }
    auto conversionResult = simdutf::convert_utf8_to_utf32_with_errors(buffer, 1, &glyphCharacter);
    if (conversionResult.error != simdutf::SUCCESS)
    {
        throw std::out_of_range("error with conversion to utf32 from utf8");
    }

    glyphKey.character = glyphCharacter;
}
