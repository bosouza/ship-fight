#ifndef TILE_SHADER_H_INCLUDED
#define TILE_SHADER_H_INCLUDED
#include <vector>
#include <string>
#include <singleton.h>
#include <util_opengl.h>
#include <common.h>
#include <texture.h>

struct textureData
{
    unsigned int VAO, textureID;
};

//tile_shader inherits the singleton behavior through CRTP
class tile_shader : public singleton<tile_shader>
{
    friend class singleton<tile_shader>;

private:
    tile_shader();
    ~tile_shader();
    unsigned int newDefaultVAO(texture_mapping mapping, texture &tex, unsigned int positionVBO);
    unsigned int
        shaderProgramID,
        EBO,
        positionXLocation, positionYLocation,
        offsetXLocation, offsetYLocation;
    std::vector<textureData> textures;

public:
    unsigned int newDiamondTexture(texture_mapping mapping, texture &tex, float width, float height);
    unsigned int newRectangularTexture(texture_mapping mapping, texture &tex, float width, float height);
    void bindTexture(unsigned int textureID);
    void draw();
    void setOriginPosition(NDC originPosition);
    void setOriginPosition(float posX, float posY);
    void setOriginPositionX(float posX);
    void setOriginPositionY(float posY);
    void setTextureOffset(float offsetX, float offsetY);
    void setTextureOffsetX(float offsetX);
    void setTextureOffsetY(float offsetY);
};

#endif