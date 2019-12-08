#include <bits/stdc++.h>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <tile_texture.h>
#include <util_opengl.h>
#include <timer.h>
#include <world_map.h>

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

#define DIAMOND_TILE_WIDTH 0.2f
#define DIAMOND_TILE_HEIGHT 0.15f

#define SQUARE_TILE_SIZE 0.25f

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    if (!glfwInit())
    {
        cout << "failed to start GLFW3" << endl;
        throw;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Ship fight", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    tile_texture ghostShipTile("./texture/ghost-ship-4-perspectives-transparent.png", 4, 4, square, SQUARE_TILE_SIZE, SQUARE_TILE_SIZE);
    tile_texture pirateShipTile("./texture/pirate-ship-4-perspectives-transparent.png", 4, 4, square, SQUARE_TILE_SIZE, SQUARE_TILE_SIZE);
    //timer maintains an update rate of 60Hz max
    timer t(60);
    world_map world(DIAMOND_TILE_WIDTH, DIAMOND_TILE_HEIGHT, &t);
    while (!glfwWindowShouldClose(window))
    {
        t.update();
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        world.draw(world_coordinates{10.0f, 50.0f});
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}