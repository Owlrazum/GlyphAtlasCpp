//
// Created by Abai on 07.02.2023.
//

#ifndef CLION_GLYPHATLAS_H
#define CLION_GLYPHATLAS_H

#include "Rect.h"
#include <vector>
#include "math.h"

using namespace std;

class GlyphAtlas
{
public:
    GlyphAtlas();

    void Update(vector<Rect> newRects);

    vector<Rect> GetRects() const
    {
        return rects;
    }

    Pair GetDims() const
    { return dims; }

private:
    vector<Rect> rects;
    Pair dims;

    static bool CompareGreater(const Rect& lhs, const Rect& rhs);
};

#endif //CLION_GLYPHATLAS_H
