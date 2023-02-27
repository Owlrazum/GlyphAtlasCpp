#include "FreeTypeWrapper.h"

#include <stdexcept>
#include <string>

FreeTypeWrapper::FreeTypeWrapper()
{
    library = {};
    face = {};
    errorCode = FT_Init_FreeType(&library);
    if (errorCode)
    {
        throw std::invalid_argument("FreeType library was not initialized " + std::to_string(errorCode));
    }
}

void FreeTypeWrapper::UseFont(const char *fontPath, int faceIndex)
{
    errorCode = FT_New_Face(library, fontPath, faceIndex, &face);
    if (errorCode == FT_Err_Unknown_File_Format )
    {
        throw std::invalid_argument("Unknown file format");
    }
    else if (errorCode)
    {
        throw std::invalid_argument("Unknown error");
    }
}

void FreeTypeWrapper::SetPixelSize(int size)
{
    errorCode = FT_Set_Char_Size(
                face,
                0,
                size,
                300,
                300);
}

FT_Bitmap FreeTypeWrapper::RenderGlyph(char character)
{
    errorCode = FT_Load_Char(face, character, FT_LOAD_RENDER);
    return face->glyph->bitmap;
}

FreeTypeWrapper::~FreeTypeWrapper()
{
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}
