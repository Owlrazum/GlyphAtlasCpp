#include "FileUtils.h"
#include "GlyphAtlas.h"
#include "CppExport.h"

#include <vector>
#include <iostream>

uint16_2 maxTextureDims = {512, 512};
GlyphAtlas glyphAtlasStepped = GlyphAtlas(maxTextureDims);

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
            glyphAtlasStepped.InitGlyphDims(pass);
            glyphAtlasStepped.Update(pass);
            glyphAtlasStepped.RemoveUnused();
        }

        glyphAtlasStepped.Render();

        auto texturesCount = glyphAtlasStepped.GetTexturesCount();
        atlasTextures->count = texturesCount;
        atlasTextures->textures = new uint8*[texturesCount];
        for (machine i = 0; i < texturesCount; i++)
        {
            atlasTextures->textures[i] = glyphAtlasStepped.GetTextureBuffer(i);
        }
    }

    DLLEXPORT void FreeAtlasTextures(AtlasTextures* atlasTextures)
    {
        // The textureBuffers themselves are taken from GlyphTexture's vector.
        // So no need to free them.
        delete atlasTextures->textures;
    }
}

int main()
{
    auto testCase = ReadGlyphKeysByLine(GetTestGlyphKeysPath(0));
    int passNumber = 0;
    for (auto& pass : testCase)
    {
        glyphAtlasStepped.InitGlyphDims(pass);
        glyphAtlasStepped.Update(pass);
        if (passNumber == 2)
        {
            std::cout<<"p=2";
        }
        glyphAtlasStepped.RemoveUnused();
        passNumber++;
    }

    glyphAtlasStepped.Render();
}