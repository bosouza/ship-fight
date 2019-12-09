#ifndef WORLD_MAP_H_INCLUDED
#define WORLD_MAP_H_INCLUDED
#include <tile_texture.h>
#include <glm/glm.hpp>
#include <tile_map.h>
#include <common.h>
#include <drawable_compenent.h>

class world_map
{
private:
    float tileWidth, tileHeight;
    // diagonalQtd is the number of tiles to render in the diagonal direction
    unsigned int diagonalQtd;
    tile_texture tile;
    tile_map map;
    glm::mat2 worldToNDCmat;
    NDC worldToNDC(world_coordinates center, world_coordinates pos);

public:
    world_map(float tileWidth, float tileHeight, timer *t);
    ~world_map(){};
    void draw(world_coordinates center);
    void draw(world_coordinates center, world_coordinates pos, drawable_component *toDraw);
    bool isNavigable(world_coordinates pos);
};

#endif