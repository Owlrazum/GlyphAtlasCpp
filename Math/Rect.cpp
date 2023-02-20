#include "Rect.h"
#include<sstream>

Rect::Rect() : x(0), y(0), w(0), h(0)
{
}

Rect::Rect(const std::string& serialized)
{
    std::istringstream in {serialized};
    std::string buf;
    in >> buf >> x >> y >> buf >> w >> h >> buf;
}

std::string Rect::ToString() const
{
    return "shelfPos: " + std::to_string(x) + " " + std::to_string(y) + " size: " + std::to_string(w) + " " + std::to_string(h);
}

Rect::Rect(ushort xArg, ushort yArg, ushort wArg, ushort hArg)
 : x(xArg), y(yArg), w(wArg), h(hArg)
{

}

void Rect::Update(ushort xArg, ushort yArg, ushort wArg, ushort hArg)
{
    this->x = xArg;
    this->y = yArg;
    this->w = wArg;
    this->h = hArg;
}
