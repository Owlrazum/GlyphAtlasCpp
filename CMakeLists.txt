cmake_minimum_required(VERSION 3.24)
project(GlyphAtlas)

set(CMAKE_CXX_STANDARD 17)

add_library(GlyphAtlas SHARED GlyphAtlas/Rect.cpp GlyphAtlas/GlyphAtlas.cpp main.cpp SvgWriter.cpp SvgWriter.h)
add_executable(GlyAtlasTest GlyphAtlas/Rect.cpp GlyphAtlas/GlyphAtlas.cpp main.cpp GlyphAtlas/Slack.h SvgWriter.cpp SvgWriter.h)
#set_target_properties(GlyphAtlas PROPERTIES BUNDLE TRUE)