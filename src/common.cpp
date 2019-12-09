#include <common.h>
#include <math.h>

bool isClose(world_coordinates a, world_coordinates b, float distance)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy) < distance;
}