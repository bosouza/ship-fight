#include <world_map.h>
#include <glm/glm.hpp>
#include <cmath>

world_map::world_map(float tileWidth, float tileHeight, timer *t)
    : tile("./texture/avalon.png", 27, 24, diamond, tileWidth, tileHeight),
      worldToNDC(tileWidth / 2, -tileHeight / 2,
                 tileWidth / 2, tileHeight / 2),
      map(t)

{
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    float hyp = std::sqrt((tileWidth / 2) * (tileWidth / 2) + (tileHeight / 2) * (tileHeight / 2));
    //we'll be drawing some tiles that fall well outside the viewport, but mehh
    this->diagonalQtd = 2 * std::sqrt(2.0f) / hyp;
}

void world_map::draw(world_coordinates center)
{
    float fractionalX = std::modf(center.x - 0.5f, nullptr);
    float fractionalY = std::modf(center.y - 0.5f, nullptr);
    int startIndex = -std::ceil((float)this->diagonalQtd / 2.0f);
    int endIndex = std::ceil((float)this->diagonalQtd / 2.0f);
    for (int x = startIndex; x < endIndex; x++)
    {
        if (center.x + x < 0)
            continue;
        for (int y = startIndex; y < endIndex; y++)
        {
            if (center.y + y < 0)
                continue;
            glm::vec2 offset = this->worldToNDC * glm::vec2(fractionalX + x, fractionalY + y);
            this->tile.draw(map.getXY(center.x + x, center.y + y), NDC{x : offset[0], y : offset[1]});
        }
    }
}