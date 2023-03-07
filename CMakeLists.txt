cmake_minimum_required(VERSION 3.24)
project(GlyphAtlas)

set(CMAKE_CXX_STANDARD 17)

add_library(FreeTypeTest SHARED Targets/FreeTypeTest.cpp)
add_library(RectAtlasTest SHARED Targets/RectAtlasTest.cpp)
add_library(GlyphAtlasTest SHARED Targets/GlyphAtlasTest.cpp)

add_executable(TestGenerator Targets/TestGenerator.cpp )
add_executable(GlyphAtlasTestTest Targets/GlyphAtlasTest.cpp)
add_executable(RectAtlasTestTest Targets/RectAtlasTest.cpp)

add_subdirectory(GlyphAtlasSrc)
add_subdirectory(FileUtils)
add_subdirectory(Math)
add_subdirectory(FreeType)
add_subdirectory(SimdUtf)
add_subdirectory(Algorithms)

#set_target_properties(GlyphAtlasSrc PROPERTIES BUNDLE TRUE)