cmake_minimum_required(VERSION 3.24)
project(GlyphAtlas)

set(CMAKE_CXX_STANDARD 17)
include_directories(GlyphAtlasSrc FileUtils GlyphAtlasSrc/Glyph Math ${CMAKE_SOURCE_DIR})

add_executable(GlyphAtlas main.cpp)
add_subdirectory(GlyphAtlasSrc)
add_subdirectory(FileUtils)
add_subdirectory(Math)

#set_target_properties(GlyphAtlasSrc PROPERTIES BUNDLE TRUE)