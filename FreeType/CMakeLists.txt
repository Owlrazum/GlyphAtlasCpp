find_package(BZip2 REQUIRED)
find_package(PNG REQUIRED)

target_link_libraries(FreeTypeTest ${BZIP2_LIBRARIES} ${PNG_LIBRARIES} /Users/Abai/Desktop/Evolve/GlyphAtlas/libfreetyped.a)
target_sources(FreeTypeTest
               PRIVATE
               FreeTypeWrapper.cpp
               )
target_include_directories(FreeTypeTest PRIVATE ${BZIP2_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS} /Users/Abai/Downloads/freetype-2.10.0/include)


target_link_libraries(GlyphAtlasTest ${BZIP2_LIBRARIES} ${PNG_LIBRARIES} /Users/Abai/Desktop/Evolve/GlyphAtlas/libfreetyped.a)
target_sources(GlyphAtlasTest
               PRIVATE
               FreeTypeWrapper.cpp
               )
target_include_directories(GlyphAtlasTest PRIVATE ${BZIP2_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS} /Users/Abai/Downloads/freetype-2.10.0/include)