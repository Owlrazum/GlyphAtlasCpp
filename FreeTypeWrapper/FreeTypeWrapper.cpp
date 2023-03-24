#include "FreeTypeWrapper.h"

#include FT_SIZES_H

#include "FileUtils.h"

#include "SimdUtfWrapper.h"

#include <stdexcept>
#include <string>
#include <cassert>

#include <iostream>
FreeTypeWrapper::FreeTypeWrapper()
{
    library = {};
    errorCode = FT_Init_FreeType(&library);
    if (errorCode)
    {
        throw std::invalid_argument("FreeTypeWrapper library was not initialized " + std::to_string(errorCode));
    }
}

void FreeTypeWrapper::InitGlyphKey(const FontKey &fontKey, GlyphKey &glyphKey)
{
    if (auto search = keysToIndexInPointers.find(fontKey); search != keysToIndexInPointers.end())
    {
        glyphKey.fontIndex = search->second;
    }
    else
    {
        glyphKey.fontIndex = AddFontKey(fontKey);
    }

    SimdUtfWrapper::ConvertToUtf32(glyphKey);
}

uint8 FreeTypeWrapper::AddFontKey(const FontKey &fontKey)
{
    if (auto pair = facesByFontFileId.find(fontKey.fileId); pair != facesByFontFileId.end())
    {
        assert(sizesByFontKey.find(fontKey) == sizesByFontKey.end());

        FT_Face& face = pair->second;
        FT_Size size;
        errorCode = FT_New_Size(face, &size);
        if (errorCode) throw std::invalid_argument("Unknown error");
        FT_Activate_Size(size);
        errorCode = FT_Set_Char_Size(face, 0, fontKey.size, dpi.x, dpi.y);
        if (errorCode) throw std::invalid_argument("Unknown error");

        sizesByFontKey.insert(std::make_pair(fontKey, size));
        facesByFontFileId.insert(std::make_pair(fontKey.fileId, face));
    }
    else
    {
        FT_Face newFace;
        errorCode = FT_New_Face(library, GetFontPath(fontKey.fileId.fontId).c_str(), fontKey.fileId.faceIndex, &newFace);
        if (errorCode == FT_Err_Unknown_File_Format) throw std::invalid_argument("Unknown file format");
        if (errorCode) throw std::invalid_argument("Unknown error");

        errorCode = FT_Set_Char_Size(newFace, 0, fontKey.size, dpi.x, dpi.y);
        if (errorCode) throw std::invalid_argument("Unknown error");

        sizesByFontKey.insert(std::make_pair(fontKey, newFace->size));
        facesByFontFileId.insert(std::make_pair(fontKey.fileId, newFace));
    }

    auto fontKeyIndex = static_cast<uint8>(pointersToKeys.size());
    auto it = keysToIndexInPointers.insert(std::make_pair(fontKey, fontKeyIndex)).first;
    pointersToKeys.push_back(&it->first);
    return fontKeyIndex;
}

FT_Bitmap FreeTypeWrapper::RenderGlyph(const GlyphKey& glyphKey)
{
    auto& fontKey = *pointersToKeys[glyphKey.fontIndex];
    if (auto pair = sizesByFontKey.find(fontKey); pair != sizesByFontKey.end())
    {
        FT_Activate_Size(pair->second);
        auto face = pair->second->face;
        errorCode = FT_Load_Char(face, glyphKey.character, FT_LOAD_RENDER);
        if (errorCode)
        {
            throw std::out_of_range("Some error occurred: " + std::to_string(errorCode));
        }
        return face->glyph->bitmap;
    }

    throw std::out_of_range("The font key was not added");
}

FreeTypeWrapper::~FreeTypeWrapper()
{
    for(auto faceByFont : facesByFontFileId)
    {
        FT_Done_Face(faceByFont.second); // the sizes were added to face's size_list by FT_New_Size
    }

    FT_Done_FreeType(library);
}