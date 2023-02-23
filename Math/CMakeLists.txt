target_sources(GlyphAtlas
               PRIVATE
               Rect.cpp
               EndPoints.cpp
               )
target_include_directories(GlyphAtlas PRIVATE .)

target_sources(GlyphAtlasLib
               PRIVATE
               Rect.cpp
               EndPoints.cpp
               )
target_include_directories(GlyphAtlasLib PRIVATE .)