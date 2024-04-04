if (UNIX)
    set(FREETYPE_LIBRARIES ${PROJECT_SOURCE_DIR}/FreeTypeWrapper/FreeType/libfreetype.a)
elseif(WIN32)
    set(FREETYPE_LIBRARIES ${PROJECT_SOURCE_DIR}/FreeTypeWrapper/FreeType/freetype.lib)
endif()

set(FREETYPE_INCLUDE ${PROJECT_SOURCE_DIR}/FreeTypeWrapper/FreeType/include)

target_link_libraries(FreeTypeTest ${FREETYPE_LIBRARIES})
target_sources(FreeTypeTest
               PRIVATE
               FreeTypeWrapper.cpp
               )
target_include_directories(FreeTypeTest PRIVATE ${FREETYPE_INCLUDE} .)

target_link_libraries(FreeTypeTestTest ${FREETYPE_LIBRARIES})
target_sources(FreeTypeTestTest
               PRIVATE
               FreeTypeWrapper.cpp
               )
target_include_directories(FreeTypeTestTest PRIVATE ${FREETYPE_INCLUDE} .)

target_link_libraries(RectAtlasTest ${FREETYPE_LIBRARIES})
target_sources(RectAtlasTest
               PRIVATE
               FreeTypeWrapper.cpp
               )
target_include_directories(RectAtlasTest PRIVATE ${FREETYPE_INCLUDE} .)

#target_link_libraries(RectAtlasTestTest ${FREETYPE_LIBRARIES})
#target_sources(RectAtlasTestTest
#               PRIVATE
#               FreeTypeWrapper.cpp
#               )
#target_include_directories(RectAtlasTestTest PRIVATE ${FREETYPE_INCLUDE} .)

target_link_libraries(GlyphAtlasTest ${FREETYPE_LIBRARIES})
target_sources(GlyphAtlasTest
               PUBLIC
               FreeTypeWrapper.cpp
               )
target_include_directories(GlyphAtlasTest PUBLIC ${FREETYPE_INCLUDE} .)

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