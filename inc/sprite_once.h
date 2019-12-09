#ifndef SPRITE_ONCE_H_INCLUDED
#define SPRITE_ONCE_H_INCLUDED
#include <tile_texture.h>
#include <common.h>
#include <timer.h>
#include <string>
#include <drawable_compenent.h>

class sprite_once : public drawable_component
{
private:
    tile_texture *tex;
    timer *t;
    float period;
    bool done;
    double creationTime;

public:
    sprite_once(tile_texture *tex, float period, timer *t);
    ~sprite_once() {}
    bool isDone();
    void draw(NDC pos);
};

#endif