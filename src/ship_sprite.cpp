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
}

void ship_sprite::bindAction(unsigned int action)
{
    if (action < 0 || action > 3)
    {
        std::cout << "the action should be 0, 1, 2 or 3. The action " << action << " doesn't exist." << std::endl;
        throw;
    }
    else
        this->currentAction = action;
}

void ship_sprite::draw(NDC pos)
{
    double elapsed = fmod(this->t->getTime(),this->period * this->frameQtd);
    unsigned int frame = 0;
    while((elapsed = elapsed - this->period) >= 0){
        frame++;
    }
    this->tex.draw(tileID{frame, this->currentAction}, pos);
}
