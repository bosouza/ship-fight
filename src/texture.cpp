#include <texture.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

texture::texture(std::string path)
{
    stbi_set_flip_vertically_on_load(true);
    this->path = path;
    this->data = stbi_load(path.c_str(), &this->width, &this->height, &this->nChannels, 0);
    if (this->data == NULL)
    {
        std::cout << "error reading file \"" << path << "\"" << std::endl;
        throw;
    }
    this->dataSize = this->width * this->height * this->nChannels * sizeof(this->data[0]);
}

void texture::freeData()
{
    if (this->data != nullptr)
    {
        stbi_image_free(this->data);
        this->data = nullptr;
    }
}

texture::~texture()
{

    this->freeData();
}