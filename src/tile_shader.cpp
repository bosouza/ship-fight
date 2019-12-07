#include <iostream>
#include <tile_shader.h>
#include <util_opengl.h>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

//defines the name of the uniform that controls the origin position for the to be drawn (NDC coordinates)
#define POSITION_X_UNIFORM "positionx"
#define POSITION_Y_UNIFORM "positiony"

//defines the name of the uniform that controls the offset for maping textures (0 to 1.0f)
#define OFFSET_X_UNIFORM "offsetx"
#define OFFSET_Y_UNIFORM "offsety"

//diamond tile size
#define DIAMOND_TILE_WIDTH 0.25f
#define DIAMOND_TILE_HEIGHT 0.15f

#define SQUARE_TILE_SIZE 0.25f

//size of the buffer containing the NDC coordinates for the shapes. No sense changing this without adding more points
#define VERTEX_DATA_BUFFER_SIZE 12

//size of the buffer containing the NDC coordinates for a texture mapping
#define TEXTURE_MAPPING_BUFFER_SIZE 8

void textureMappingToBuffer(texture_mapping mapping, float buffer[]);
void fillDiamondShapeBuffer(float buffer[], float width, float height);
void fillRectangleShapeBuffer(float buffer[], float width, float height);

tile_shader::tile_shader()
{
    //load the shaders, create a shader program linking them, make sure it compiles
    unsigned int vertShaderID = createVertexShader("./shaders/tile_shader.vert");
    unsigned int fragShaderID = createFragShader("./shaders/tile_shader.frag");
    this->shaderProgramID = glCreateProgram();
    unsigned int p = this->shaderProgramID;
    glAttachShader(p, vertShaderID);
    glAttachShader(p, fragShaderID);
    glLinkProgram(p);
    testCompileProgram(p);
    glUseProgram(p);
    glDeleteShader(vertShaderID);
    glDeleteShader(fragShaderID);

    //get the location for all uniforms we're using in the shaders
    this->positionXLocation = glGetUniformLocation(p, POSITION_X_UNIFORM);
    this->positionYLocation = glGetUniformLocation(p, POSITION_Y_UNIFORM);
    this->offsetXLocation = glGetUniformLocation(p, OFFSET_X_UNIFORM);
    this->offsetYLocation = glGetUniformLocation(p, OFFSET_Y_UNIFORM);

    // create VBO for the "diamond" shape, bind the buffer data
    float diamondShapeBuffer[VERTEX_DATA_BUFFER_SIZE];
    fillDiamondShapeBuffer(diamondShapeBuffer, DIAMOND_TILE_WIDTH, DIAMOND_TILE_HEIGHT);
    this->diamondVBO = createVBO(diamondShapeBuffer, VERTEX_DATA_BUFFER_SIZE);

    //create VBO for the square shape, bind the buffer data
    float squareShapeBuffer[VERTEX_DATA_BUFFER_SIZE];
    fillRectangleShapeBuffer(squareShapeBuffer, SQUARE_TILE_SIZE, SQUARE_TILE_SIZE);
    this->squareVBO = createVBO(squareShapeBuffer, VERTEX_DATA_BUFFER_SIZE);

    //create EBO.
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3};
    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

tile_shader::~tile_shader()
{
    //TODO: cleanup shader resources
}

unsigned int tile_shader::newDiamondTexture(texture_mapping mapping, texture &tex)
{
    return newDefaultVAO(mapping, tex, this->diamondVBO);
}

unsigned int tile_shader::newSquareTexture(texture_mapping mapping, texture &tex)
{
    return newDefaultVAO(mapping, tex, this->squareVBO);
}

unsigned int tile_shader::newDefaultVAO(texture_mapping mapping, texture &tex, unsigned int positionVBO)
{
    //create VAO and bind our EBO
    unsigned int VAO = createAndBindVAO();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

    //bind positionVBO to VAO. That's location 0 in vertex shader
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    //create and bind textureVBO to VAO. That's location 1 in vertex shader, used in frag shader
    float buffer[TEXTURE_MAPPING_BUFFER_SIZE];
    textureMappingToBuffer(mapping, buffer);
    unsigned int textureVBO = createVBO(buffer, TEXTURE_MAPPING_BUFFER_SIZE);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    unsigned int format = tex.getNChannels() == 4 ? GL_RGBA : GL_RGB;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, format, tex.getWidth(), tex.getHeight(), 0, format, GL_UNSIGNED_BYTE, tex.getDataPtr());

    unsigned int id = this->textures.size();
    this->textures.push_back(textureData{VAO : VAO, textureID : textureID});
    return id;
}

void tile_shader::bindTexture(unsigned int id)
{
    textureData texture = this->textures[id];
    glBindTexture(GL_TEXTURE_2D, texture.textureID);
    glBindVertexArray(texture.VAO);
}

void tile_shader::draw()
{
    glUseProgram(this->shaderProgramID);
    // TODO: assert that the currently bound VAO is one created by this shader
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

//uniform setters
void tile_shader::setOriginPositionX(float posX)
{
    glUniform1f(this->positionXLocation, posX);
}
void tile_shader::setOriginPositionY(float posY)
{
    glUniform1f(this->positionYLocation, posY);
}
void tile_shader::setTextureOffsetX(float offsetX)
{
    glUniform1f(this->offsetXLocation, offsetX);
}
void tile_shader::setTextureOffsetY(float offsetY)
{
    glUniform1f(this->offsetYLocation, offsetY);
}

void textureMappingToBuffer(texture_mapping mapping, float buffer[])
{
    float b[TEXTURE_MAPPING_BUFFER_SIZE] = {
        mapping.bottom_left.x, mapping.bottom_left.y,
        mapping.upper_left.x, mapping.upper_right.y,
        mapping.upper_right.x, mapping.upper_right.y,
        mapping.bottom_right.x, mapping.bottom_right.y};
    for (int i = 0; i < TEXTURE_MAPPING_BUFFER_SIZE; i++)
        buffer[i] = b[i];
}

void fillDiamondShapeBuffer(float buffer[], float width, float height)
{
    float b[VERTEX_DATA_BUFFER_SIZE] = {
        -width / 2, 0, 0,
        0, height / 2, 0,
        width / 2, 0, 0,
        0, -height / 2, 0};
    for (int i = 0; i < VERTEX_DATA_BUFFER_SIZE; i++)
        buffer[i] = b[i];
}

void fillRectangleShapeBuffer(float buffer[], float width, float height)
{
    float b[VERTEX_DATA_BUFFER_SIZE] = {
        -width / 2, -height / 2, 0,
        -width / 2, height / 2, 0,
        width / 2, height / 2, 0,
        width / 2, -height / 2, 0};
    for (int i = 0; i < VERTEX_DATA_BUFFER_SIZE; i++)
        buffer[i] = b[i];
}

//uniform helper setters
void tile_shader::setOriginPosition(NDC originPosition)
{
    this->setOriginPositionX(originPosition.x);
    this->setOriginPositionY(originPosition.y);
}
void tile_shader::setOriginPosition(float posX, float posY)
{
    this->setOriginPositionX(posX);
    this->setOriginPositionY(posY);
}
void tile_shader::setTextureOffset(float offsetX, float offsetY)
{
    this->setTextureOffsetX(offsetX);
    this->setTextureOffsetY(offsetY);
}