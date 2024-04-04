target_sources(RectAtlasTest
               PRIVATE
               GlyphAtlas.cpp
               Shelf.cpp
               GlyphTexture.cpp
               )
target_include_directories(RectAtlasTest PRIVATE . Glyph)

#target_sources(RectAtlasTestTest
#               PRIVATE
#               GlyphAtlas.cpp
#               Shelf.cpp
#               GlyphTexture.cpp
#               )
#target_include_directories(RectAtlasTestTest PRIVATE . Glyph)

target_sources(GlyphAtlasTest
               PRIVATE
               GlyphAtlas.cpp
               Shelf.cpp
               GlyphTexture.cpp
               )
target_include_directories(GlyphAtlasTest PRIVATE . Glyph)

target_include_directories(FreeTypeTest PRIVATE . Glyph)
target_include_directories(FreeTypeTestTest PRIVATE . Glyph)

target_sources(GlyphAtlasTestTest
               PRIVATE
               GlyphAtlas.cpp
               Shelf.cpp
               GlyphTexture.cpp)
target_include_directories(GlyphAtlasTestTest PRIVATE . Glyph)

target_include_directories(TestGenerator PRIVATE Glyph)

add_subdirectory(Stepped)