#include <ship_sprite.h>

ship_sprite::ship_sprite(std::string path, int frameQtd, int actionQtd, float width, float height, float period, timer *t)
    : tex(path, frameQtd, actionQtd, square, width, height)
{
}

void ship_sprite::bindAction(unsigned int action)
{
    this->currentAction = action;
}

void ship_sprite::draw(NDC pos)
{
    this->tex.draw(tileID{0, this->currentAction}, pos);
}