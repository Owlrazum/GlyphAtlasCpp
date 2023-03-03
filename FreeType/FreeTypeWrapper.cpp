#include "FreeTypeWrapper.h"

#include "FileUtils.h"

#include <stdexcept>
#include <string>

FreeTypeWrapper::FreeTypeWrapper()
{
    library = {};
    errorCode = FT_Init_FreeType(&library);
    if (errorCode)
    {
        throw std::invalid_argument("FreeType library was not initialized " + std::to_string(errorCode));
    }
}

// if already added, do nothing
void FreeTypeWrapper::AddFont(FontKey fontKey)
{
    if (auto search = facesByFont.find(fontKey); search != facesByFont.end())
    {
        return;
    }

    FT_Face newFace;
    errorCode = FT_New_Face(library, GetFontPath(fontKey.fontId).c_str(), fontKey.faceIndex, &newFace);
    if (errorCode == FT_Err_Unknown_File_Format )
    {
        throw std::invalid_argument("Unknown file format");
    }
    else if (errorCode)
    {
        throw std::invalid_argument("Unknown error");
    }

    errorCode = FT_Set_Char_Size(
            newFace,
            0,
            fontKey.size,
            300,
            300);

//    errorCode = FT_Set_Char_Size(
//            newFace,
//            0,
//            16 * 64,
//            96,
//            96);

    if (errorCode)
    {
        throw std::out_of_range("Some error occurred: " + std::to_string(errorCode));
    }

    facesByFont.insert(std::make_pair(fontKey, newFace));
}

FT_Bitmap FreeTypeWrapper::RenderChar(const FontKey &fontKey, uint32 character)
{
    if (auto search = facesByFont.find(fontKey); search != facesByFont.end())
    {
        errorCode = FT_Load_Char(search->second, character, FT_LOAD_RENDER);
        if (errorCode)
        {
            throw std::out_of_range("Some error occurred: " + std::to_string(errorCode));
        }
        return search->second->glyph->bitmap;
    }

    throw std::out_of_range("The font key was not added");
}

FT_Bitmap FreeTypeWrapper::RenderGlyphIndex(const FontKey &fontKey, machine glyphIndex)
{
    if (auto search = facesByFont.find(fontKey); search != facesByFont.end())
    {
        errorCode = FT_Load_Glyph(search->second, glyphIndex, FT_LOAD_RENDER);
        if (errorCode)
        {
            throw std::out_of_range("Some error occurred: " + std::to_string(errorCode));
        }
        return search->second->glyph->bitmap;
    }

    throw std::out_of_range("The font key was not added");
}

FT_Bitmap FreeTypeWrapper::RenderGlyph(const GlyphKey& glyphKey)
{
    FontKey fontKey {static_cast<uint8_t>(glyphKey.fontIndex), 0, defaultFontSize};
    return RenderChar(fontKey, glyphKey.glyphIndex);
}

FreeTypeWrapper::~FreeTypeWrapper()
{
    for(auto faceByFont : facesByFont)
    {
        FT_Done_Face(faceByFont.second);
    }

    FT_Done_FreeType(library);
}