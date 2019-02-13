#include "Core.h"
#include <iostream>
#include <unordered_set>
#include <vector>

#undef main

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


#include "Renderer.hpp"
#include "ResourceManager.hpp"

std::unordered_set<std::string> mCommands;
int main(int ac, char **av)
{
    static_cast<void>(ac);
    Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT, "Prototype");
    std::string exePath(av[0]);
    std::string exeFolder(exePath.substr(0, exePath.find_last_of("\\/") + 1));
    RESOURCES.setBinFolder(exeFolder);
    RESOURCES.loadShader("model_core.vx.glsl", "model_core.ft.glsl", "modelShader");
    RESOURCES.loadShader("sprite.vx.glsl", "sprite.ft.glsl", "sprite");
    RESOURCES.loadTexture("block.png", "block", false);

    renderer.initSpriteRenderer();

    bool isRunning = true;
    while (isRunning)
    {
        std::string input = renderer.processInput(isRunning);
        if (!input.empty())
        {
            mCommands.insert(std::string(input));
        }
        renderer.preFrame();
        renderer.frame();
        renderer.postFrame();
    }
    return 0;
}
