#ifndef COMMON_H_DEFINED
#define COMMON_H_DEFINED
#include <string>
#include <cstdio>
#include <util_opengl.h>

struct tileID
{
    unsigned int line, column;
};

struct tileProperties
{
    bool isNavigable;
};

struct texture_mapping
{
    NDC bottom_left;
    NDC upper_left;
    NDC upper_right;
    NDC bottom_right;
};

struct Position2D
{
    float x;
    float y;
};

#endif