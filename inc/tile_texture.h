#ifndef TILE_TEXTURE_H_INCLUDED
#define TILE_TEXTURE_H_INCLUDED
#include <texture.h>
#include <string>
#include <tile_shader.h>
#include <common.h>

typedef enum
{
    square,
    diamond,
} shape;

class tile_texture
{
private:
    int tileWidth, tileHeight, wQtd, hQtd;
    texture tex;
    tile_shader *shader;
    unsigned int textureID;

public:
    // path is the path for the image to be loaded, wQtd and hQtd are the
    // number of tiles along the width and height lengths, respectively.
    // s is the shape to be drawn
    tile_texture(std::string path, int wQtd, int hQtd, shape s, float width, float height);
    ~tile_texture(){};
    void draw(tileID tile, NDC pos);
};

#endif