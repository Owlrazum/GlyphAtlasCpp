#pragma once

#include "PrimitiveTypes.h"

enum TypeMask {
    Sdf,
    Msdf,
    Raster,
    RasterOutline,
};

struct FontFileId
{
    uint8 fontId;
    uint8 faceIndex;

    bool operator<(const FontFileId &rhs) const
    {
        if (fontId < rhs.fontId)
            return true;
        if (rhs.fontId < fontId)
            return false;
        return faceIndex < rhs.faceIndex;
    }

    bool operator>(const FontFileId &rhs) const
    {
        return rhs < *this;
    }

    bool operator<=(const FontFileId &rhs) const
    {
        return !(rhs < *this);
    }

    bool operator>=(const FontFileId &rhs) const
    {
        return !(*this < rhs);
    }
};

struct FontKey {
    FontFileId fileId{};
    int32 size{}; // FT_F26Dot6 aka signed long. Lazy to include the freeType library here.

    // not sure how to use these yet, and whether they should participate in the map keys.
    // float sdfPadding;
    // TypeMask mask;

    bool operator<(const FontKey &rhs) const
    {
        if (fileId < rhs.fileId)
            return true;
        if (rhs.fileId < fileId)
            return false;
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