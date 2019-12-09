#include <ship.h>
#include <math.h>

ship::ship(float positionx, float positiony, float speed, float angle, float angularVelocity, int lives)
    : game_entity(positionx, positiony, speed, angle, angularVelocity)
{
    this->sinked = false;
    this->damage = 0;
    this->lives = lives;
}

void ship::hit()
{
    this->damage++;
    if (damage > lives)
        this->sinked = true;
}