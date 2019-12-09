#ifndef WORLD_MAP_H_INCLUDED
#define WORLD_MAP_H_INCLUDED
#include <tile_texture.h>
#include <glm/glm.hpp>
#include <tile_map.h>
#include <common.h>

class world_map
{
private:
    float tileWidth, tileHeight;
    // diagonalQtd is the number of tiles to render in the diagonal direction
    unsigned int diagonalQtd;
    tile_texture tile;
    tile_map map;
    glm::mat2 worldToNDC;

public:
    world_map(float tileWidth, float tileHeight, timer *t);
    ~world_map(){};
    void draw(world_coordinates center);
};

#endif