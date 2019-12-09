#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED
#include <game_entity.h>

class ship : public game_entity
{
private:
    int damage;
    int lives;

public:
    bool sinked;
    ship(float positionx, float positiony, float speed, float angle, float angularVelocity, int lives);
    ~ship() {}
    void hit();
};

#endif