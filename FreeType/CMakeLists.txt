SET(CMAKE_EXE_LINKER_FLAGS "-v")
SET(FT_DISABLE_HARFBUZZ OFF BOOL)

find_package(Freetype REQUIRED)
if(Freetype_FOUND)
    message(STATUS "Freetype:    YES (ver ${Freetype_VERSION})")
else()
    message(STATUS "Freetype:    NO")
endif()

find_package(BZip2 REQUIRED)
if(BZip2_FOUND)
    message(STATUS "BZip2:    YES (ver ${BZip2_VERSION})")
else()
    message(STATUS "BZip2:    NO")
endif()

find_package(PNG REQUIRED)
if(PNG_FOUND)
    message(STATUS "PNG:    YES (ver ${PNG_VERSION})")
else()
    message(STATUS "PNG:    NO")
endif()

#find_package(harfbuzz REQUIRED)
#if(harfbuzz_FOUND)
#    message(STATUS "harfbuzz:    YES (ver ${harfbuzz_VERSION}) (lib ${harfbuzz_LIBRARIES})")
#else()
#    message(STATUS "harfbuzz:    NO")
#endif()

target_link_libraries(FreeTypeTest ${FREETYPE_LIBRARIES} ${BZIP2_LIBRARIES} ${PNG_LIBRARIES})
target_include_directories(FreeTypeTest PRIVATE ${FREETYPE_INCLUDE_DIRS} ${BZIP_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS})

#target_link_libraries(FreeTypeTest ${harfbuzz_LIBRARIES})
#target_include_directories(FreeTypeTest PRIVATE ${harfbuzz_INCLUDE_DIRS})

target_sources(FreeTypeTest
               PRIVATE
               FreeTypeWrapper.cpp
               )