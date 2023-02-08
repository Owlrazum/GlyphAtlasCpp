//
//
//
//
//
// Created by Abai on 07.02.2023.
//

#ifndef CLION_RECT_H
#define CLION_RECT_H

#include <string>
#include <format>

class Rect
{
public:
    int x;
    int y;

    int w;
    int h;

    Rect();

    std::string ToString();
};


#endif //CLION_RECT_H
