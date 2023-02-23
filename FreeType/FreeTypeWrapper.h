#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

class FreeTypeWrapper
{
public:
    FreeTypeWrapper();

private:
    FT_Library library;
};