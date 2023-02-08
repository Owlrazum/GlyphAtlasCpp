//
// Created by Abai on 07.02.2023.
//

#include "Rect.h"

Rect::Rect() : x(0), y(0), w(0), h(0)
{
}

std::string Rect::ToString()
{
    return "pos: " + std::to_string(x) + " " + std::to_string(y) + "; size: " + std::to_string(w) + " " + std::to_string(h)+ ";";
}
