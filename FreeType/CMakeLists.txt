find_package(BZip2 REQUIRED)
find_package(PNG REQUIRED)

set(FREETYPE_LIBRARIES ${BZIP2_LIBRARIES} ${PNG_LIBRARIES} /Users/Abai/Desktop/Evolve/GlyphAtlas/FreeType/libfreetyped.a)
set(FREETYPE_INCLUDE ${BZIP2_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS} /Users/Abai/Downloads/freetype-2.10.0/include)

target_link_libraries(FreeTypeTest ${FREETYPE_LIBRARIES})
target_sources(FreeTypeTest
               PRIVATE
               FreeTypeWrapper.cpp
               )
target_include_directories(FreeTypeTest PRIVATE ${FREETYPE_INCLUDE} .)

target_link_libraries(RectAtlasTest ${FREETYPE_LIBRARIES})
target_sources(RectAtlasTest
               PRIVATE
               FreeTypeWrapper.cpp
               )
target_include_directories(RectAtlasTest PRIVATE ${FREETYPE_INCLUDE} .)

target_link_libraries(RectAtlasTestTest ${FREETYPE_LIBRARIES})
target_sources(RectAtlasTestTest
               PRIVATE
               FreeTypeWrapper.cpp
               )
target_include_directories(RectAtlasTestTest PRIVATE ${FREETYPE_INCLUDE} .)

target_link_libraries(GlyphAtlasTest ${FREETYPE_LIBRARIES})
target_sources(GlyphAtlasTest
               PRIVATE
               FreeTypeWrapper.cpp
               )
target_include_directories(GlyphAtlasTest PRIVATE ${FREETYPE_INCLUDE} .)

target_link_libraries(GlyphAtlasTestTest ${FREETYPE_LIBRARIES})
target_sources(GlyphAtlasTestTest
               PRIVATE
               FreeTypeWrapper.cpp
               )
target_include_directories(GlyphAtlasTestTest PRIVATE ${FREETYPE_INCLUDE} .)

target_link_libraries(TestGenerator ${FREETYPE_LIBRARIES})
target_sources(TestGenerator
               PRIVATE
               FreeTypeWrapper.cpp
               )
target_include_directories(TestGenerator PRIVATE ${FREETYPE_INCLUDE} .)