#include <sprite_once.h>
#include <cmath>
#include <iostream>

sprite_once::sprite_once(tile_texture *tex, float period, timer *t)
{
    this->period = period;
    this->t = t;
    this->done = false;
    this->creationTime = t->getTime();
    this->tex = tex;
    std::cout << "created" << std::endl;
}

void sprite_once::draw(NDC pos)
{
    double elapsedTime = this->t->getTime() - this->creationTime;
    // not sure why this isn't working, so I'll just hardcode y wayout of it
    // double animationPeriod = this->period * this->tex->wQtd * this->tex->hQtd;
    double animationPeriod = this->period * 16;
    // std::cout << "getTime: " << this->t->getTime() << " creationTime: " << this->creationTime << std::endl;
    // std::cout << "elapsedTime: " << elapsedTime << " animationPeriod: " << animationPeriod << std::endl;
    // std::cout << "this->period: " << this->period << " this->tex->wQtd: " << this->tex->wQtd << " this->tex->hQtd: " << this->tex->hQtd << std::endl;
    if (done || (elapsedTime > animationPeriod))
    {
        std::cout << "done" << std::endl;
        done = true;
        return;
    }
    //TODO: figure out why I can't reader wQtd and hQtd
    // unsigned int line = (unsigned int)((this->t->getTime() - this->creationTime) / (this->period * this->tex->wQtd));
    // unsigned int column = (unsigned int)(fmod((this->t->getTime() - this->creationTime), this->period * this->tex->wQtd));
    unsigned int line = (unsigned int)((this->t->getTime() - this->creationTime) / (this->period * 4));
    unsigned int column = (unsigned int)(fmod((this->t->getTime() - this->creationTime), this->period * 4));
    this->tex->draw(tileID{line, column}, pos);
}

bool sprite_once::isDone()
{
    return this->done;
}