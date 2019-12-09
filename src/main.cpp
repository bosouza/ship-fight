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
#include <button_util.h>
#include <bullet.h>
#include <ship.h>
#include <ship_sprite.h>
#include <explosion.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 500

#define DIAMOND_TILE_WIDTH 0.2f
#define DIAMOND_TILE_HEIGHT 0.15f

#define SQUARE_TILE_SIZE 0.25f

#define PI 3.1415f
#define SHIP_SPEED 3
#define SHIP_LIVES 5
#define BULLET_SPEED 15
#define BULLET_TRAVEL_DISTANCE 10
#define BULLET_SIZE 0.05f
#define HIT_DISTANCE 0.3f
//no animation, our ship textures are too garbage for that
#define SHIP_ANIMATION_PERIOD 0
#define EXPLOSION_SIZE 0.15f
#define BULLET_EXPLOSION_PERIOD 0.01f

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

ship player[] = {ship(15, 15, 0, 0, 0, SHIP_LIVES),
                 ship(20, 20, 0, 0, 0, SHIP_LIVES)};
unsigned int playerAction[] = {0, 0};
press_event buttons[2];

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

    timer t(60);
    ship_sprite ghostShip("./texture/ghost-ship-4-perspectives-transparent.png", 4, 4, SQUARE_TILE_SIZE, SQUARE_TILE_SIZE, SHIP_ANIMATION_PERIOD, &t);
    ship_sprite pirateShip("./texture/pirate-ship-4-perspectives-transparent.png", 4, 4, SQUARE_TILE_SIZE, SQUARE_TILE_SIZE, SHIP_ANIMATION_PERIOD, &t);
    tile_texture bulletExplosionTile("./texture/explosion-bullet.png", 4, 4, square, EXPLOSION_SIZE, EXPLOSION_SIZE);
    tile_texture bulletTile("./texture/cannon-ball.png", 1, 1, square, BULLET_SIZE, BULLET_SIZE);
    bulletTile.setTile(tileID{0, 0});
    //timer maintains an update rate of 60Hz max
    list<bullet> bullets;
    list<explosion> explosions;
    world_map world(DIAMOND_TILE_WIDTH, DIAMOND_TILE_HEIGHT, &t);
    while (!glfwWindowShouldClose(window))
    {
        t.update();
        // cout << "Draw loop, elapsedTime: " << t.getElapsedTime() << endl;
        processInput(window);

        //update player's position
        for (int i = 0; i < 2; i++)
        {
            if (player[i].sinked)
                continue;
            ship pCopy = player[i];
            player[i].step(t.getElapsedTime());
            if (!world.isNavigable(player[i].position))
                player[i] = pCopy;
        }

        // add new bullets
        for (int i = 0; i < 2; i++)
        {
            if (buttons[i].pressEvent())
            {
                if (player[i].sinked)
                    continue;
                bullets.push_back(bullet(player[i].position.x, player[i].position.y, BULLET_SPEED, player[i].angle, 0, i, BULLET_TRAVEL_DISTANCE));
            }
        }

        // update bullets
        auto bulletI = bullets.begin();
        while (bulletI != bullets.end())
        {
            bulletI->step(t.getElapsedTime());
            if (!bulletI->isAlive())
            {
                bulletI = bullets.erase(bulletI);
                continue;
            }
            if (isClose(bulletI->position, player[bulletI->player == 0 ? 1 : 0].position, HIT_DISTANCE))
            {
                explosions.push_back(explosion(bulletI->position, &bulletExplosionTile, BULLET_EXPLOSION_PERIOD, &t));
                player[bulletI->player == 0 ? 1 : 0].hit();
                bulletI = bullets.erase(bulletI);
                continue;
            }
            ++bulletI;
        }

        // update explosions
        auto explosionI = explosions.begin();
        while (explosionI != explosions.end())
        {
            if (explosionI->isDone())
            {
                explosionI = explosions.erase(explosionI);
                continue;
            }
            ++explosionI;
        }

        //DEBUG: remove background color once we don't need it anymore
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw background
        for (int i = 0; i < 2; i++)
        {
            bindViewport(viewports[i]);
            world.draw(world_coordinates{player[i].position.x, player[i].position.y});
        }

        // draw ships
        for (int i = 0; i < 2; i++)
        {
            bindViewport(viewports[i]);
            if (!player[i].sinked)
            {
                if (i == 0)
                {
                    ghostShip.bindAction(playerAction[0]);
                    ghostShip.draw(NDC{0, 0});
                }
                else
                {
                    pirateShip.bindAction(playerAction[1]);
                    pirateShip.draw(NDC{0, 0});
                }
            }
            if (!player[i == 0 ? 1 : 0].sinked)
            {
                if (i == 0)
                    world.draw(player[0].position, player[1].position, &pirateShip);
                else
                    world.draw(player[1].position, player[0].position, &ghostShip);
            }
        }

        //draw bullets
        for (int i = 0; i < 2; i++)
        {
            bindViewport(viewports[i]);
            for (auto bulletI = bullets.begin(); bulletI != bullets.end(); bulletI++)
            {
                world.draw(player[i].position, bulletI->position, &bulletTile);
            }
        }

        //draw explosions
        for (int i = 0; i < 2; i++)
        {
            bindViewport(viewports[i]);
            for (auto explosionI = explosions.begin(); explosionI != explosions.end(); explosionI++)
            {
                world.draw(player[i].position, explosionI->pos, &(*explosionI));
            }
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
        playerAction[0] = 3;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        player[0].speed = SHIP_SPEED;
        player[0].angle = 5 * PI / 4;
        playerAction[0] = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        player[0].speed = SHIP_SPEED;
        player[0].angle = 7 * PI / 4;
        playerAction[0] = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        player[0].speed = SHIP_SPEED;
        player[0].angle = 1 * PI / 4;
        playerAction[0] = 2;
    }
    player[1].speed = 0;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        player[1].speed = SHIP_SPEED;
        player[1].angle = 3 * PI / 4;
        playerAction[1] = 3;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        player[1].speed = SHIP_SPEED;
        player[1].angle = 5 * PI / 4;
        playerAction[1] = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        player[1].speed = SHIP_SPEED;
        player[1].angle = 7 * PI / 4;
        playerAction[1] = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        player[1].speed = SHIP_SPEED;
        player[1].angle = 1 * PI / 4;
        playerAction[1] = 2;
    }
    buttons[0].setState(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
    buttons[1].setState(glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
}