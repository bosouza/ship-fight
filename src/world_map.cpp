#include <world_map.h>
#include <glm/glm.hpp>
#include <cmath>

world_map::world_map(float tileWidth, float tileHeight, timer *t)
    : tile("./texture/avalon.png", 27, 24, diamond, tileWidth, tileHeight),
      worldToNDCmat(tileWidth / 2, -tileHeight / 2,
                    tileWidth / 2, tileHeight / 2),
      map(t)

{
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    float hyp = std::sqrt((tileWidth / 2) * (tileWidth / 2) + (tileHeight / 2) * (tileHeight / 2));
    //we'll be drawing some tiles that fall well outside the viewport, but mehh
    // this->diagonalQtd = 2 * std::sqrt(2.0f) / hyp;
    this->diagonalQtd = 3 * std::sqrt(2.0f) / hyp;
}

void world_map::draw(world_coordinates center)
{
    int viewRadius = std::ceil((float)this->diagonalQtd / 2.0f);
    for (int x = std::max((int)center.x - viewRadius, 0);
         x < std::min((int)center.x + viewRadius, (int)map.getWidth());
         x += 1)
    {
        for (int y = std::max((int)center.y - viewRadius, 0);
             y < std::min((int)center.y + viewRadius, (int)map.getHeight());
             y += 1)
        {
            NDC targetPos = this->worldToNDC(center, world_coordinates{(float)x, (float)y});
            this->tile.draw(map.getXY(x, y), targetPos);
        }
    }
}

void world_map::draw(world_coordinates center, world_coordinates pos, drawable_component *toDraw)
{
    NDC targetPos = this->worldToNDC(center, pos);
    toDraw->draw(targetPos);
}

NDC world_map::worldToNDC(world_coordinates center, world_coordinates pos)
{
    glm::vec2 res = this->worldToNDCmat * glm::vec2(pos.x - center.x, pos.y - center.y);
    return NDC{res[0], res[1]};
}

bool world_map::isNavigable(world_coordinates pos)
{
    tileID tile = this->map.getXY(pos.x, pos.y);
    tileProperties p = map.getProperties(tile);
    return p.isNavigable;
}