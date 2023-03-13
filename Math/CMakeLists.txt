target_sources(TestGenerator
               PRIVATE
               Rect.cpp
               )
target_include_directories(TestGenerator PRIVATE .)

target_sources(FreeTypeTest
               PRIVATE
               Rect.cpp
               )
target_include_directories(FreeTypeTest PRIVATE .)

target_sources(RectAtlasTest
               PRIVATE
               Rect.cpp
               )
target_include_directories(RectAtlasTest PRIVATE .)

target_sources(RectAtlasTestTest
               PRIVATE
               Rect.cpp
               )
target_include_directories(RectAtlasTestTest PRIVATE .)

target_sources(GlyphAtlasTest
               PRIVATE
               Rect.cpp
               )
target_include_directories(GlyphAtlasTest PRIVATE .)

target_sources(GlyphAtlasTestTest
               PRIVATE
               Rect.cpp
               )
target_include_directories(GlyphAtlasTestTest PRIVATE .)