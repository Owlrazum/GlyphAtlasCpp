#include "FileUtils.h"
#include "GlyphAtlas.h"
#include "CppExport.h"

#include "FreeTypeWrapper.h"

#include <vector>

FreeTypeWrapper freeType;
GlyphAtlas glyphAtlas = GlyphAtlas({512, 512});

struct GlyphData
{
    Glyph *glyphs;
    unsigned char **bitmaps;
    int *rowCounts;
    int *pitches;

    int glyphsCount;
};

GlyphData glyphData;

extern "C"
{
    // returns number of glyphs
    DLLEXPORT GlyphData* RenderGlyphs(int testNumber)
    {
        auto keys = ReadGlyphKeysByLine(GetTestGlyphKeysPath(testNumber));

        long fontSize = 16 * 32;

        std::set<GlyphKey> renderedGlyphs;

        for (const auto& keysPass : keys)
        {
            std::vector<std::pair<GlyphKey, Glyph>> updateGlyphs (keys.size());
            for (const auto& key: keysPass)
            {
                if (renderedGlyphs.find(key) != renderedGlyphs.end())
                {
                    if (glyphAtlas.MarkIfContainsGlyph(key)) // without mark the glyph will be deleted as not used
                    {
                        continue;
                    }
                }
                renderedGlyphs.insert(key);
                
                FontKey fontKey {key.fontIndex, 0, fontSize};
                freeType.AddFont(fontKey);
                // We need just the dimensions of the bitmap, not render it wholly.
                // todo: check the existence of function for returning just dimensions.
                auto bitmap = freeType.RenderGlyph(fontKey, key.glyphIndex);
                Rect rect {0, 0, static_cast<ushort>(bitmap.width), static_cast<ushort>(bitmap.rows)};
                Glyph glyph {0, rect};
                updateGlyphs.emplace_back(key, glyph);
            }
            glyphAtlas.Update(updateGlyphs);
        }


        int glyphsCount = glyphAtlas.GetPlacedGlyphsCount();
        auto* glyphs = new Glyph[glyphsCount];
        auto* bitmaps = new unsigned char*[glyphsCount];
        auto* rowCounts = new int[glyphsCount];
        auto* pitches = new int[glyphsCount];

        int index = 0;
        int textureCount = glyphAtlas.GetTextureCount();
        for (int i = 0; i < textureCount; i++)
        {
            auto textureGlyphs = glyphAtlas.GetGlyphsFromTexture(i);
            for (auto pair : textureGlyphs)
            {
                auto& glyphKey = pair.first;
                FontKey fontKey {glyphKey.fontIndex, 0, fontSize};
                auto bitmap = freeType.RenderGlyph(fontKey, glyphKey.glyphIndex);
                glyphs[index] = pair.second;
                std::copy(bitmap.buffer, bitmap.buffer + bitmap.pitch * bitmap.rows, bitmaps[index]);
                rowCounts[index] = static_cast<int>(bitmap.rows);
                pitches[index] = bitmap.pitch;
                index++;
            }
        }
        glyphData = {glyphs, bitmaps, pitches, rowCounts, glyphsCount};
        return &glyphData;
    }

    DLLEXPORT GlyphData FreeData()
    {
        free(glyphData.glyphs);
        for (int i = 0; i < glyphData.glyphsCount; i++)
        {
            free(glyphData.bitmaps[i]);
        }
        free(glyphData.bitmaps);
        free(glyphData.rowCounts);
        free(glyphData.pitches);
    }
}