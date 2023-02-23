target_sources(GlyphAtlas
               PRIVATE
               SvgWriter.cpp
               FileUtils.cpp
               )
target_include_directories(GlyphAtlas PRIVATE .)

target_sources(GlyphAtlasLib
               PRIVATE
               FileUtils.cpp
               )
target_include_directories(GlyphAtlasLib PRIVATE .)
