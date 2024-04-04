cmake_minimum_required(VERSION 3.24)
project(GlyphAtlas)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_SHARED_LIBS "Whether to build shared .dll libs" ON)

add_compile_definitions(PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

# link options: /VERBOSE:LIB
#add_link_options(/NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libcmtd.lib /NODEFAULTLIB:msvcrtd.lib)
link_libraries(/usr/local/Cellar/libpng/1.6.43/lib/libpng.a)
link_libraries(/usr/local/Cellar/bzip2/1.0.8/lib/libbz2.a)
link_libraries(/usr/local/Cellar/zlib/1.3.1/lib/libz.a)

add_library(FreeTypeTest Targets/FreeTypeTest.cpp)
add_library(RectAtlasTest Targets/RectAtlasTest.cpp)
add_library(GlyphAtlasTest Targets/GlyphAtlasTest.cpp)

add_executable(GlyphAtlasTestTest Targets/GlyphAtlasTest.cpp)
#add_executable(RectAtlasTestTest Targets/RectAtlasTest.cpp)
add_executable(FreeTypeTestTest Targets/FreeTypeTest.cpp)
add_executable(TestGenerator Targets/TestGenerator.cpp )

add_subdirectory(GlyphAtlasSrc)
add_subdirectory(FileUtils)
add_subdirectory(Math)
add_subdirectory(FreeTypeWrapper)
add_subdirectory(SimdUtfWrapper)
add_subdirectory(Algorithms)