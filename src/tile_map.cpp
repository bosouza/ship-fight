#include <tile_map.h>
#include <timer.h>
#include <common.h>
#include <sstream>
#include <cmath>

tile_map::tile_map(timer* tim)
{
    this->tmx.load("./tiled/newMap.tmx");
    this->tsx.load(tmx.tilesetList[0].source.c_str());
    this->t = tim;

    std::stringstream  ss(this->tmx.tileLayer["Tile Layer 1"].data.contents);
    std::string substr;
    for(int l=0;l<this->tmx.mapInfo.height;l++){
        std::vector<unsigned int> record;
        for(int c=0;c<this->tmx.mapInfo.width;c++){
            if(ss.good()){
                getline( ss, substr, ',' );
                record.push_back(std::atoi( substr.c_str() ) - 1);
            }
        }
        this->tilesScenario.push_back(record);
    }
}

tileID tile_map::get(unsigned int line, unsigned int column){
    struct tileID tile_id;
    unsigned int tile = this->tilesScenario[line][column];

    if(this->tsx.tileList[tile].hasAnimation){
        double elapsedAnimation = fmod(this->t->getTime() * 1000,this->tsx.tileList[tile].period);
        unsigned int frame = 0;
        while((elapsedAnimation = elapsedAnimation - this->tsx.tileList[tile].animation_tile_id_duration[frame])>=0){
            frame++;
        }
     tile = this->tsx.tileList[tile].animation_tile_id[frame]+1;
    }
    else
        tile += 1;

    unsigned int line_aux = tile/this->tsx.tileset.clmns;
    unsigned int column_aux = tile - (line_aux * this->tsx.tileset.clmns) - 1;
    
    tile_id.line = line_aux;
    tile_id.column = column_aux;
    return tile_id;
}

tileID tile_map::getXY(unsigned int x, unsigned int y){
    unsigned int line = (this->tilesScenario.size() - 1) - y;
    return tile_map::get(line,x);
}

tileProperties tile_map::getProperties(tileID tile_id){
    struct tileProperties tile_properties;
    unsigned int tile = tile_id.line * this->tsx.tileset.clmns;
    tile += tile_id.column;

    std::string value = this->tsx.tileList[tile].property["navigable"];
    if(value=="false") tile_properties.isNavigable = false;
    else tile_properties.isNavigable = true;

    return tile_properties;
}

unsigned int tile_map::getWidth(){
    return this->tmx.mapInfo.width;
}

unsigned int tile_map::getHeight(){
    return this->tmx.mapInfo.height;
}
