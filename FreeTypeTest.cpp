#include "FreeTypeWrapper.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>

int main()
{
    FreeTypeWrapper freeType;

    FT_Library  library;
    FT_Face face;
    auto error = FT_Init_FreeType( &library );
    error = FT_New_Face( library,
                   "../Fonts/Lato-Regular.ttf",
                   0,
                   &face );
    std::cout << error << std::endl;

    FT_Done_Face(face);
    FT_Done_FreeType(library);
    return 0;
}