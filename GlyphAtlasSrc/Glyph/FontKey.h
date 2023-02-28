#pragma once

enum TypeMask {
    Sdf,
    Msdf,
    Raster,
    RasterOutline,
};

struct FontKey {
    uint8_t fontId{};
    uint8_t faceIndex{};
    long size{}; // FT_F26Dot6. Lazy to include the freeType library here.
//    float sdfPadding; // not sure how to use these yet, and whether they should participate in the map keys.
//    TypeMask mask;

    bool operator<(const FontKey &rhs) const
    {
        if (fontId < rhs.fontId) return true;
        if (rhs.fontId < fontId) return false;
        if (faceIndex < rhs.faceIndex) return true;
        if (rhs.faceIndex < faceIndex) return false;

        return size < rhs.size;
    }

    bool operator>(const FontKey &rhs) const
    {
        return rhs < *this;
    }

    bool operator<=(const FontKey &rhs) const
    {
        return !(rhs < *this);
    }

    bool operator>=(const FontKey &rhs) const
    {
        return !(*this < rhs);
    }
};