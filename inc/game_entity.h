#ifndef GAME_ENTITY_H_DEFINED
#define GAME_ENTITY_H_DEFINED
#include <common.h>

class game_entity
{
protected:
    float travelledDistance;

public:
    float speed;
    float angle;
    float angularVelocity;
    world_coordinates position;
    game_entity(float positionx, float positiony, float speed, float angle, float angularVelocity);
    ~game_entity() {}
    void step(float time);
};

#endif