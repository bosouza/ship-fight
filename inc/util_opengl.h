#ifndef UTIL_OPENGL_H_DEFINED
#define UTIL_OPENGL_H_DEFINED
#include <string>
#include <utility>

//normalized device coordinate
struct NDC
{
    float x, y;
};

typedef struct
{
    std::pair<int, int> lowerLeftCorner;
    int width;
    int height;
} viewport;

unsigned int createVertexShader(const char *path);
unsigned int createFragShader(const char *path);
unsigned int createShader(unsigned int type, const char *path);
std::string testCompileProgram(unsigned int program);
// void testCompileProgram(unsigned int program);
unsigned int createVBO(float buffer[], unsigned int size);
unsigned int createAndBindVAO();
void bindViewport(viewport v);

#endif