#ifndef TILE_MAP_H_DEFINED
#define TILE_MAP_H_DEFINED
#include <string>
#include <timer.h>
#include <common.h>

class tile_map
{
private:
public:
    tile_map(timer &t);
    tileID get(unsigned int line, unsigned int column);
    tileID getXY(unsigned int x, unsigned int y);
    tileProperties getProperties(tileID);
    unsigned int getWidth();
    unsigned int getHeight();
    ~tile_map();
};
#endif