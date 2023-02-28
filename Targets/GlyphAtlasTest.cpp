#include "FileUtils.h"
#include "GlyphAtlas.h"
#include "CppExport.h"

#include "FreeTypeWrapper.h"

#include <vector>

FreeTypeWrapper freeType;
Pair maxTextureDims = {512, 512};
GlyphAtlas glyphAtlas = GlyphAtlas(maxTextureDims);

struct AtlasTextures
{
    unsigned char **textures;
    int *widths;
    int *heights;
    int count;
};

AtlasTextures atlasTextures;

extern "C"
{
    // returns number of glyphs
    DLLEXPORT AtlasTextures* RenderGlyphs(int testNumber)
    {
        auto testCase = ReadGlyphKeysByLine(GetTestGlyphKeysPath(testNumber));
        for (auto& pass : testCase)
        {
            glyphAtlas.InitGlyphDims(pass);
            glyphAtlas.Update(pass);
        }

        glyphAtlas.Render();
        int texturesCount = glyphAtlas.GetTexturesCount();

        auto textures = new unsigned char *[texturesCount];
        auto width = new int[texturesCount];
        auto heights = new int[texturesCount];

        for (int i = 0; i < texturesCount; i++)
        {
            textures[i] = glyphAtlas.GetTextureBuffer(i);
            width[i] = maxTextureDims.x;
            heights[i] = maxTextureDims.y;
        }

        atlasTextures = {textures, width, heights, texturesCount};
        return &atlasTextures;
    }

    DLLEXPORT void FreeData()
    {
        // The textureBuffers themselves are taken from GlyphTexture's vector.
        // So no need to free them.
        delete atlasTextures.textures;
        delete atlasTextures.widths;
        delete atlasTextures.heights;
    }
}