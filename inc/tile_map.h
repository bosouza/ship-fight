#ifndef TILE_MAP_H_DEFINED
#define TILE_MAP_H_DEFINED
#include <timer.h>
#include <common.h>
#include <TMXParser.h>
#include <TSXParser.h>

class tile_map
{
private:
    TMX::Parser tmx;
    TSX::Parser tsx;
    timer* t;
    std::vector<std::vector<unsigned int>> tilesScenario;
public:
    tile_map(timer* tim);
    tileID get(unsigned int line, unsigned int column);
    tileID getXY(unsigned int x, unsigned int y);
    tileProperties getProperties(tileID);
    unsigned int getWidth();
    unsigned int getHeight();
    ~tile_map(){};
};
#endif