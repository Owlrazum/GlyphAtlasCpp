target_sources(TestGenerator
               PRIVATE
               Rect.cpp
               EndPoints.cpp
               )
target_include_directories(TestGenerator PRIVATE .)

target_sources(FreeTypeTest
               PRIVATE
               Rect.cpp
               EndPoints.cpp
               )
target_include_directories(FreeTypeTest PRIVATE .)

target_sources(RectAtlasTest
               PRIVATE
               Rect.cpp
               EndPoints.cpp
               )
target_include_directories(RectAtlasTest PRIVATE .)

target_sources(GlyphAtlasTest
               PRIVATE
               Rect.cpp
               EndPoints.cpp
               )
target_include_directories(GlyphAtlasTest PRIVATE .)

target_sources(GlyphAtlasTestTest
               PRIVATE
               Rect.cpp
               EndPoints.cpp
               )
target_include_directories(GlyphAtlasTestTest PRIVATE .)