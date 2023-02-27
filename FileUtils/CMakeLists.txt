target_sources(TestGenerator
               PRIVATE
               SvgWriter.cpp
               FileUtils.cpp
               )
target_include_directories(RectAtlasTest PRIVATE .)

target_sources(RectAtlasTest
               PRIVATE
               FileUtils.cpp
               )
target_include_directories(RectAtlasTest PRIVATE .)

target_sources(GlyphAtlasTest
               PRIVATE
               FileUtils.cpp
               )
target_include_directories(GlyphAtlasTest PRIVATE .)
