#ifndef EXPLOSION_H_INCLUDED
#define EXPLOSION_H_INCLUDED
#include <sprite_once.h>
#include <common.h>
class explosion : public sprite_once
{
private:
public:
    world_coordinates pos;
    explosion(world_coordinates pos, tile_texture *tex, float period, timer *t);
    ~explosion() {}
};

explosion::explosion(world_coordinates pos, tile_texture *tex, float period, timer *t)
    : sprite_once(tex, period, t)
{
    this->pos = pos;
}

#endif