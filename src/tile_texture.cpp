#include <tile_texture.h>
#include <tile_shader.h>
#include <iostream>

texture_mapping createTextureMapping(float width, float height);

tile_texture::tile_texture(std::string path, int wQtd, int hQtd, shape s, float width, float height) : tex(path)
{
    if (wQtd < 1 || hQtd < 1)
    {
        std::cout << "should have at least one tile in both horizontal and vertical directions" << std::endl;
        throw;
    }

    if (this->tex.getWidth() % wQtd != 0)
    {
        std::cout << "image width should be a multiple of tile width" << std::endl;
        throw;
    }

    if (this->tex.getHeight() % hQtd != 0)
    {
        std::cout << "image height should be a multiple of tile height" << std::endl;
        throw;
    }

    this->wQtd = wQtd;
    this->hQtd = hQtd;
    this->tileWidth = this->tex.getWidth() / wQtd;
    this->tileHeight = this->tex.getHeight() / hQtd;
    this->currentTile = tileID{0, 0};

    this->shader = &tile_shader::get_instance();
    texture_mapping textureMapping = createTextureMapping(1.0f / (float)this->wQtd, 1.0f / (float)this->hQtd);
    switch (s)
    {
    case diamond:
        this->textureID = this->shader->newDiamondTexture(textureMapping, this->tex, width, height);
        break;
    case square:
        this->textureID = this->shader->newRectangularTexture(textureMapping, this->tex, width, height);
        break;
    default:
        std::cout << "unsupported shape" << std::endl;
        throw;
    }
    this->tex.freeData();
}

void tile_texture::draw(tileID tile, NDC pos)
{
    this->setTile(tile);
    this->draw(pos);
}

void tile_texture::setTile(tileID tile)
{
    this->currentTile = tile;
}

void tile_texture::draw(NDC pos)
{
    if (this->currentTile.column >= this->wQtd)
        throw "tried to draw a tile column outside the texture range\n";
    if (this->currentTile.line >= this->hQtd)
        throw "tried to draw a tile line outside the texture range\n";
    this->shader->setOriginPosition(pos);
    float tileBottomLeftCornerX = this->currentTile.column * this->tileWidth;
    float tileBottomLeftCornerY = (this->hQtd - this->currentTile.line - 1) * this->tileHeight;
    this->shader->setTextureOffsetX((float)tileBottomLeftCornerX / (float)this->tex.getWidth());
    this->shader->setTextureOffsetY((float)tileBottomLeftCornerY / (float)this->tex.getHeight());
    this->shader->bindTexture(this->textureID);
    this->shader->draw();
}

texture_mapping createTextureMapping(float width, float height)
{
    texture_mapping mapping = {
        bottom_left : {0, 0},
        upper_left : {0, height},
        upper_right : {width, height},
        bottom_right : {width, 0},
    };
    return mapping;
}