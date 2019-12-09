#ifndef SHIP_SPRITE_H_INCLUDED
#define SHIP_SPRITE_H_INCLUDED
#include <timer.h>
#include <tile_texture.h>
#include <string>
class ship_sprite
{
private:
    tile_texture tex;
    unsigned int currentAction;
    float period;
    timer *t;
    int frameQtd;
    int actionQtd;
public:
    ship_sprite(std::string path, int frameQtd, int actionQtd, float width, float height, float period, timer *t);
    void bindAction(unsigned int action);
    void draw(NDC pos);
    ~ship_sprite(){};
};

#endif
