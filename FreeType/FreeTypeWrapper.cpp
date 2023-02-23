#include "FreeTypeWrapper.h"

#include <stdexcept>
#include <string>


FreeTypeWrapper::FreeTypeWrapper()
{
    library = {};
    auto error = FT_Init_FreeType(&library);
    if (error)
    {
        throw std::invalid_argument("FreeType library was not initialized " + std::to_string(error));
    }
}
