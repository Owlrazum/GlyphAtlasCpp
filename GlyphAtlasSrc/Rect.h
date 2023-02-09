//
//
//
//
//
// Created by Abai on 07.02.2023.
//

#ifndef CLION_RECT_H
#define CLION_RECT_H

#ifndef STRING_H
#define STRING_H
#include <string>
#endif

#ifndef TUPLE_H
#define TUPLE_H
#include "Tuple.h"
#endif

class Rect
{
public:
    ushort x;
    ushort y;

    ushort w;
    ushort h;

    Rect();
    Rect(ushort xArg, ushort yArg, ushort wArg, ushort hArg);
    Rect(std::string);

    int GetArea() { return w * h;}
    void Update(ushort x, ushort y, ushort w, ushort h);

    std::string ToString();
};


#endif //CLION_RECT_H
