#pragma once

enum TypeMask {
    Sdf,
    Msdf,
    Raster,
    RasterOutline,
};

struct FontKey {
    std::byte fontId;
    float fontSize;
    float sdfPadding;
    TypeMask mask;
};