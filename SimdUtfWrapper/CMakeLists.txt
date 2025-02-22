if (UNIX)
    set(SIMDUTF_LIBRARY ${PROJECT_SOURCE_DIR}/SimdUtfWrapper/SimdUtf/libsimdutf.a)
elseif(WIN32)
    set(SIMDUTF_LIBRARY ${PROJECT_SOURCE_DIR}/SimdUtfWrapper/SimdUtf/simdutf.lib)
endif()

set(SIMDUTF_INCLUDE ${PROJECT_SOURCE_DIR}/SimdUtfWrapper/SimdUtf/include)

target_link_libraries(RectAtlasTest ${SIMDUTF_LIBRARY})
target_sources(RectAtlasTest
               PRIVATE
               SimdUtfWrapper.cpp
               )
target_include_directories(RectAtlasTest PRIVATE . ${SIMDUTF_INCLUDE})

#target_link_libraries(RectAtlasTestTest ${SIMDUTF_LIBRARY})
#target_sources(RectAtlasTestTest
#               PRIVATE
#               SimdUtfWrapper.cpp
#               )
#target_include_directories(RectAtlasTestTest PRIVATE . ${SIMDUTF_INCLUDE})

target_link_libraries(GlyphAtlasTest ${SIMDUTF_LIBRARY})
target_sources(GlyphAtlasTest
               PRIVATE
               SimdUtfWrapper.cpp
               )
target_include_directories(GlyphAtlasTest PRIVATE . ${SIMDUTF_INCLUDE})

target_link_libraries(GlyphAtlasTestTest ${SIMDUTF_LIBRARY})
target_sources(GlyphAtlasTestTest
               PRIVATE
               SimdUtfWrapper.cpp
               )
target_include_directories(GlyphAtlasTestTest PRIVATE . ${SIMDUTF_INCLUDE})

target_link_libraries(FreeTypeTest ${SIMDUTF_LIBRARY})
target_sources(FreeTypeTest
               PRIVATE
               SimdUtfWrapper.cpp
               )
target_include_directories(FreeTypeTest PRIVATE . ${SIMDUTF_INCLUDE})

target_link_libraries(FreeTypeTestTest ${SIMDUTF_LIBRARY})
target_sources(FreeTypeTestTest
               PRIVATE
               SimdUtfWrapper.cpp
               )
target_include_directories(FreeTypeTestTest PRIVATE . ${SIMDUTF_INCLUDE})

target_link_libraries(TestGenerator ${SIMDUTF_LIBRARY})
target_sources(TestGenerator
               PRIVATE
               SimdUtfWrapper.cpp
               )
target_include_directories(TestGenerator PRIVATE . ${SIMDUTF_INCLUDE})