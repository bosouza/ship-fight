#include <ship_sprite.h>
#include <cmath>
#include <iostream>

ship_sprite::ship_sprite(std::string path, int frameQtd, int actionQtd, float width, float height, float period, timer *t)
    : tex(path, frameQtd, actionQtd, square, width, height)
{
    this->t = t;
    this->frameQtd = frameQtd;
    this->period = period;
    this->actionQtd = actionQtd;
    this->currentAction = 1;
}

void ship_sprite::bindAction(unsigned int action)
{
    if (action < 0 || action >= this->actionQtd)
    {
        std::cout << "the action " << action << " doesn't exist." << std::endl;
        throw;
    }
    else
        this->currentAction = action;
}

void ship_sprite::draw(NDC pos)
{
    unsigned int frame = 0;
    if (this->period != 0)
    {
        double elapsed = fmod(this->t->getTime(), this->period * this->frameQtd);
        while ((elapsed = elapsed - this->period) >= 0)
        {
            frame++;
        }
    }
    this->tex.draw(tileID{this->currentAction, frame}, pos);
}
