#include <util_opengl.h>
#include <bits/stdc++.h>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

std::string readFile(const char *filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open())
    {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

void testCompileShader(unsigned int shader)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "shader compilation failed" << std::endl
                  << infoLog << std::endl;
        throw;
    }
}

//TODO: make this a void function (for some reason the glProgram doesn't compile if I change the return type)
std::string testCompileProgram(unsigned int program)
{
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_COMPILE_STATUS, &success);
    std::cout << "success: " << success << std::endl;
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "program compilation failed" << std::endl
                  << infoLog << std::endl;
        throw;
    }
    return "";
}

// void testCompileProgram(unsigned int program)
// {
//     int success;
//     char infoLog[512];
//     glGetProgramiv(program, GL_COMPILE_STATUS, &success);
//     std::cout << "success: " << success << std::endl;
//     if (!success)
//     {
//         glGetProgramInfoLog(program, 512, NULL, infoLog);
//         std::cout << "program compilation failed" << std::endl
//                   << infoLog << std::endl;
//         throw;
//     }
// }

unsigned int createVertexShader(const char *path)
{
    return createShader(GL_VERTEX_SHADER, path);
}

unsigned int createFragShader(const char *path)
{
    return createShader(GL_FRAGMENT_SHADER, path);
}

unsigned int createShader(unsigned int type, const char *path)
{
    std::string src = readFile(path);
    const char *src_ptr = src.c_str();
    unsigned int shaderID;
    shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &src_ptr, NULL);
    glCompileShader(shaderID);
    try
    {
        testCompileShader(shaderID);
    }
    catch (std::string e)
    {
        std::cout << path << ": " << e;
        throw;
    }
    return shaderID;
}

unsigned int createVBO(float buffer[], unsigned int size)
{
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), buffer, GL_STATIC_DRAW);
    return VBO;
}

unsigned int createAndBindVAO()
{
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    return VAO;
}