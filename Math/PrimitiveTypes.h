#pragma once

#if defined(_MSC_VER)

typedef signed char             int8;
typedef short              int16;
typedef int                   int32;
typedef __int64                int64;
typedef unsigned char        uint8;
typedef unsigned short       uint16;
typedef unsigned int         uint32;
typedef unsigned __int64      uint64;

#if defined(_M_X64)

    typedef __int64             machine;
    typedef unsigned __int64   umachine;
    typedef unsigned __int64   machine_address;

#else

    typedef int                machine;
    typedef unsigned int      umachine;
    typedef unsigned int      machine_address;

#endif


#elif defined(__ORBIS__) || defined(__PROSPERO__)

typedef signed char             int8;
typedef short              int16;
typedef int                   int32;
typedef long               int64;
typedef unsigned char        uint8;
typedef unsigned short       uint16;
typedef unsigned int         uint32;
typedef unsigned long        uint64;
typedef long               machine;
typedef unsigned long        umachine;
typedef unsigned long        machine_address;

#elif defined(__GNUC__)

typedef signed char int8;
typedef short int16;
typedef int int32;
typedef long long int64;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

#if defined(__LP64__) || defined(_WIN64)

typedef long long machine;
typedef unsigned long long umachine;
typedef unsigned long long machine_address;

#else

typedef int    machine;
typedef unsigned int umachine;
typedef unsigned int machine_address;

#endif
#endif

// define size_t without using another header
#ifndef size_t
using x = decltype(alignof(char));
typedef x size_t;
#endif

#include <ostream>

struct uint16_2
{
    uint16_2(uint16 xArg, uint16 yArg) : x(xArg), y(yArg) {}

    uint16_2()
    {
        x = 0; y = 0;
    }

    uint16 x;
    uint16 y;
    friend std::ostream& operator<<(std::ostream& out, uint16_2& pair)
    {
        return out << pair.x << " " << pair.y;
    }
};

struct uint16_3
{
    uint16 x;
    uint16 y;
    uint16 z;
};

struct uint16_4
{
    uint16 x;
    uint16 y;
    uint16 z;
    uint16 w;
};