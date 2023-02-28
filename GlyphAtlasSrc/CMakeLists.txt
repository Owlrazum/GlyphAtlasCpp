target_sources(TestGenerator
               PRIVATE
               GlyphAtlas.cpp
               Shelf.cpp
               GlyphTexture.cpp
               )
target_include_directories(TestGenerator PRIVATE . Glyph)

target_sources(RectAtlasTest
               PRIVATE
               GlyphAtlas.cpp
               Shelf.cpp
               GlyphTexture.cpp
               )
target_include_directories(RectAtlasTest PRIVATE . Glyph)

target_sources(GlyphAtlasTest
               PRIVATE
               GlyphAtlas.cpp
               Shelf.cpp
               GlyphTexture.cpp
               )
target_include_directories(GlyphAtlasTest PRIVATE . Glyph)

target_include_directories(FreeTypeTest PRIVATE . Glyph)