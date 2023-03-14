#include "FileUtils.h"
#include "GlyphAtlas.h"
#include "CppExport.h"

#include <vector>
#include <iostream>

namespace GlyphAtlasTest
{
    uint16_2 maxTextureDims = {512, 512};
    GlyphAtlas glyphAtlas = GlyphAtlas(maxTextureDims);

    struct AtlasTextures
    {
        uint8 **textures;
        machine count;
    };

    extern "C"
    {
    // returns number of glyphs
    DLLEXPORT void RenderAtlasTextures(AtlasTextures* atlasTextures, machine testNumber)
    {
        auto testCase = ReadGlyphKeysByLine(GetTestGlyphKeysPath(testNumber));
        for (auto& pass : testCase)
        {
            glyphAtlas.InitGlyphDims(pass);
            glyphAtlas.Update(pass);
            glyphAtlas.RemoveUnused();
        }

        glyphAtlas.Render();

        auto texturesCount = glyphAtlas.GetTexturesCount();
        atlasTextures->count = texturesCount;
        atlasTextures->textures = new uint8*[texturesCount];
        for (machine i = 0; i < texturesCount; i++)
        {
            atlasTextures->textures[i] = glyphAtlas.GetTextureBuffer(i);
        }
    }

    DLLEXPORT void FreeAtlasTextures(AtlasTextures* atlasTextures)
    {
        // The textureBuffers themselves are taken from GlyphTexture's vector.
        // So no need to free them.
        delete atlasTextures->textures;
    }
    }
}

int main()
{
    auto testCase = ReadGlyphKeysByLine(GetTestGlyphKeysPath(0));
    int passNumber = 0;
    for (auto& pass : testCase)
    {
        GlyphAtlasTest::glyphAtlas.InitGlyphDims(pass);
        GlyphAtlasTest::glyphAtlas.Update(pass);
        if (passNumber == 2)
        {
            std::cout<<"p=2";
        }
        GlyphAtlasTest::glyphAtlas.RemoveUnused();
        passNumber++;
    }

    GlyphAtlasTest::glyphAtlas.Render();
}
