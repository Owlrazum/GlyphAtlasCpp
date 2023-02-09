//
// Created by Abai on 07.02.2023.
//

#include "Rect.h"
#include<sstream>

Rect::Rect() : x(0), y(0), w(0), h(0)
{
}

Rect::Rect(std::string serialized)
{
    std::istringstream in {serialized};
    std::string buf;
    in >> buf >> x >> y >> buf >> w >> h >> buf;
}

std::string Rect::ToString()
{
    return "pos: " + std::to_string(x) + " " + std::to_string(y) + " size: " + std::to_string(w) + " " + std::to_string(h)+ "\n";
}

Rect::Rect(ushort xArg, ushort yArg, ushort wArg, ushort hArg)
 : x(xArg), y(yArg), w(wArg), h(hArg)
{

}

void Rect::Update(ushort x, ushort y, ushort w, ushort h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}
