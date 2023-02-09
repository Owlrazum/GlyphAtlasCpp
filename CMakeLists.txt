cmake_minimum_required(VERSION 3.24)
project(GlyphAtlas)

set(CMAKE_CXX_STANDARD 17)
include_directories(GlyphAtlasSrc SVGWriterSrc ${CMAKE_SOURCE_DIR})

add_executable(GlyphAtlas main.cpp Tuple.h)
add_subdirectory(GlyphAtlasSrc)
add_subdirectory(SVGWriterSrc)

#set_target_properties(GlyphAtlasSrc PROPERTIES BUNDLE TRUE)