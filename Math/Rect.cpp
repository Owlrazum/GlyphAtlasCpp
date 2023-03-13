#include "Rect.h"
#include<sstream>

Rect::Rect() : CRect()
{
    x = 0; y = 0; w = 0; h= 0;
}

Rect::Rect(const std::string &serialized) : CRect()
{
    std::istringstream in{serialized};
    std::string buf;
    in >> buf >> x >> y >> buf >> w >> h >> buf;
}

std::string Rect::ToString() const
{
    return "pos: " + std::to_string(x) + " " + std::to_string(y) + " size: " + std::to_string(w) + " " +
           std::to_string(h);
}

Rect::Rect(uint16 xArg, uint16 yArg, uint16 wArg, uint16 hArg) : CRect()
{
    x = xArg;
    y = yArg;
    w = wArg;
    h = hArg;
}

bool CRect::operator<(const CRect &rhs) const
{
    if (x < rhs.x)
        return true;
    if (rhs.x < x)
        return false;
    if (y < rhs.y)
        return true;
    if (rhs.y < y)
        return false;
    if (w < rhs.w)
        return true;
    if (rhs.w < w)
        return false;
    return h < rhs.h;
}

bool CRect::operator>(const CRect &rhs) const
{
    return rhs < *this;
}

bool CRect::operator<=(const CRect &rhs) const
{
    return !(rhs < *this);
}

bool CRect::operator>=(const CRect &rhs) const
{
    return !(*this < rhs);
}
