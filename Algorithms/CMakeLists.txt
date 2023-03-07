target_sources(TestGenerator
               PRIVATE
               Algorithms.cpp
               )
target_include_directories(TestGenerator PRIVATE .)

target_sources(FreeTypeTest
               PRIVATE
               Algorithms.cpp
               )
target_include_directories(FreeTypeTest PRIVATE .)

target_sources(RectAtlasTest
               PRIVATE
               Algorithms.cpp
               )
target_include_directories(RectAtlasTest PRIVATE .)

target_sources(RectAtlasTestTest
               PRIVATE
               Algorithms.cpp
               )
target_include_directories(RectAtlasTestTest PRIVATE .)

target_sources(GlyphAtlasTest
               PRIVATE
               Algorithms.cpp
               )
target_include_directories(GlyphAtlasTest PRIVATE .)

target_sources(GlyphAtlasTestTest
               PRIVATE
               Algorithms.cpp
               )
target_include_directories(GlyphAtlasTestTest PRIVATE .)