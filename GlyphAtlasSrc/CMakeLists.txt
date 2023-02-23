target_sources(GlyphAtlas
               PRIVATE
               GlyphAtlas.cpp
               Shelf.cpp
               GlyphTexture.cpp
               )
target_include_directories(GlyphAtlas PRIVATE . Glyph)

target_sources(GlyphAtlasLib
               PRIVATE
               GlyphAtlas.cpp
               Shelf.cpp
               GlyphTexture.cpp
               )
target_include_directories(GlyphAtlasLib PRIVATE . Glyph)