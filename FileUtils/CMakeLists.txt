target_sources(TestGenerator
               PRIVATE
               FileUtils.cpp
               )
target_include_directories(TestGenerator PRIVATE .)

target_sources(FreeTypeTest
               PRIVATE
               FileUtils.cpp
               )
target_include_directories(FreeTypeTest PRIVATE .)

target_sources(RectAtlasTest
               PRIVATE
               FileUtils.cpp
               )
target_include_directories(RectAtlasTest PRIVATE .)

target_sources(RectAtlasTestTest
               PRIVATE
               FileUtils.cpp
               )
target_include_directories(RectAtlasTestTest PRIVATE .)

target_sources(GlyphAtlasTest
               PRIVATE
               FileUtils.cpp
               )
target_include_directories(GlyphAtlasTest PRIVATE .)

target_sources(GlyphAtlasTestTest
               PRIVATE
               FileUtils.cpp
               )
target_include_directories(GlyphAtlasTestTest PRIVATE .)