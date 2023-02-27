cmake_minimum_required(VERSION 3.24)
project(GlyphAtlas)

set(CMAKE_CXX_STANDARD 17)
include_directories(FreeType FreeType/LibraryInclude FreeType/LibraryInclude/freetype)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

add_executable(TestGenerator Targets/TestGenerator.cpp)
add_library(FreeTypeTest SHARED Targets/FreeTypeTest.cpp)
add_library(RectAtlasTest SHARED Targets/RectAtlasTest.cpp)
add_library(GlyphAtlasTest SHARED Targets/GlyphAtlasTest.cpp)

add_subdirectory(GlyphAtlasSrc)
add_subdirectory(FileUtils)
add_subdirectory(Math)
add_subdirectory(FreeType)

#set_target_properties(GlyphAtlasSrc PROPERTIES BUNDLE TRUE)