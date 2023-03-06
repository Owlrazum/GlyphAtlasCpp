#include "FreeTypeWrapper.h"

#include FT_SIZES_H

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
    if (auto pair = facesByFont.find(fontKey.fileId); pair != facesByFont.end())
    {
        if (auto size = sizesByFontKey.find(fontKey); size != sizesByFontKey.end())
        {
            return;
        }

        FT_Size size;
        errorCode = FT_New_Size(pair->second, &size);
        if (errorCode) throw std::invalid_argument("Unknown error");
        FT_Activate_Size(size);
        errorCode = FT_Set_Char_Size(pair->second, 0, fontKey.size, dpi.x, dpi.y);
        if (errorCode) throw std::invalid_argument("Unknown error");
    }

    FT_Face newFace;
    errorCode = FT_New_Face(library, GetFontPath(fontKey.fileId.fontId).c_str(), fontKey.fileId.faceIndex, &newFace);
    if (errorCode)
    {
        if (errorCode == FT_Err_Unknown_File_Format)
        {
            throw std::invalid_argument("Unknown file format");
        }
        throw std::invalid_argument("Unknown error");
    }

    errorCode = FT_Set_Char_Size(newFace, 0, fontKey.size, dpi.x, dpi.y);
    if (errorCode) throw std::invalid_argument("Unknown error");

    sizesByFontKey.insert(std::make_pair(fontKey, newFace->size));
    facesByFont.insert(std::make_pair(fontKey.fileId, newFace));
}

FT_Bitmap FreeTypeWrapper::RenderChar(const FontKey &fontKey, uint32 character)
{
    if (auto pair = sizesByFontKey.find(fontKey); pair != sizesByFontKey.end())
    {
        auto face = pair->second->face;
        errorCode = FT_Load_Char(face, character, FT_LOAD_RENDER);
        if (errorCode)
        {
            throw std::out_of_range("Some error occurred: " + std::to_string(errorCode));
        }
        return face->glyph->bitmap;
    }

    throw std::out_of_range("The font key was not added");
}

FT_Bitmap FreeTypeWrapper::RenderGlyph(const GlyphKey& glyphKey)
{
    FontKey fontKey {glyphKey.fontIndex, 0, defaultFontSize};
    return RenderChar(fontKey, glyphKey.glyphIndex);
}

FreeTypeWrapper::~FreeTypeWrapper()
{
    for(auto faceByFont : facesByFont)
    {
        FT_Done_Face(faceByFont.second); // the sizes were added to face's size_list by FT_New_Size
    }

    FT_Done_FreeType(library);
}