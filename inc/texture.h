#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED
#include <string>
class texture
{
private:
    int width, height, nChannels, dataSize;
    unsigned char *data;
    std::string path;

public:
    texture(std::string path);
    ~texture();
    void freeData();
    int getWidth() { return this->width; }
    int getHeight() { return this->height; }
    int getNChannels() { return this->nChannels; }
    unsigned char *getDataPtr() { return this->data; }
    std::string getPath() { return this->path; }
};

#endif