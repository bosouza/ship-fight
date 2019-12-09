#include <bits/stdc++.h>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <tile_texture.h>
#include <util_opengl.h>
#include <timer.h>
#include <world_map.h>
#include <game_entity.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 500

#define DIAMOND_TILE_WIDTH 0.2f
#define DIAMOND_TILE_HEIGHT 0.15f

#define SQUARE_TILE_SIZE 0.25f

#define PI 3.1415f
#define SHIP_SPEED 3

using namespace std;
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

viewport viewports[] = {
    {
        lowerLeftCorner : {0, 0},
        width : WINDOW_WIDTH / 2,
        height : WINDOW_HEIGHT,
    },
    {
        lowerLeftCorner : {(WINDOW_WIDTH / 2) + 1, 0},
        width : WINDOW_WIDTH / 2,
        height : WINDOW_HEIGHT,
    }};

game_entity player[] = {game_entity(20, 50, 0, 0, 0),
                        game_entity(25, 55, 0, 0, 0)};

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
    cout << "t.getTime(): " << t.getTime() << endl;
    world_map world(DIAMOND_TILE_WIDTH, DIAMOND_TILE_HEIGHT, &t);
    while (!glfwWindowShouldClose(window))
    {
        t.update();
        // cout << "Draw loop, elapsedTime: " << t.getElapsedTime() << endl;
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (auto &p : player)
        {
            p.step(t.getElapsedTime());
        }
        for (int i = 0; i < 2; i++)
        {
            bindViewport(viewports[i]);
            world.draw(world_coordinates{player[i].position.x, player[i].position.y});
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    viewports[0] = viewport{
        lowerLeftCorner : {0, 0},
        width : width / 2,
        height : height
    };
    viewports[1] = viewport{
        lowerLeftCorner : {(width / 2) + 1, 0},
        width : width / 2,
        height : height
    };
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // I'm deeply embarrassed to write this
    player[0].speed = 0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        player[0].speed = SHIP_SPEED;
        player[0].angle = 3 * PI / 4;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        player[0].speed = SHIP_SPEED;
        player[0].angle = 5 * PI / 4;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        player[0].speed = SHIP_SPEED;
        player[0].angle = 7 * PI / 4;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        player[0].speed = SHIP_SPEED;
        player[0].angle = 1 * PI / 4;
    }
    player[1].speed = 0;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        player[1].speed = SHIP_SPEED;
        player[1].angle = 3 * PI / 4;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        player[1].speed = SHIP_SPEED;
        player[1].angle = 5 * PI / 4;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        player[1].speed = SHIP_SPEED;
        player[1].angle = 7 * PI / 4;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        player[1].speed = SHIP_SPEED;
        player[1].angle = 1 * PI / 4;
    }
}