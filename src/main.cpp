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
    std::string exePath(av[0]);
    std::string exeFolder(exePath.substr(0, exePath.find_last_of("\\/") + 1));
    RESOURCES.setBinFolder(exeFolder);
    RESOURCES.loadShader("model_core.vx.glsl", "model_core.ft.glsl", "modelShader");
    RESOURCES.loadShader("sprite.vx.glsl", "sprite.ft.glsl", "sprite");
    RESOURCES.loadTexture("block.png", "block", false);
    bool isRunning = true;
    {
        Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT, "Prototype");



        renderer.setProjection();
        while (isRunning)
        {
            std::string input = renderer.processInput(isRunning);
            if (!input.empty())
            {
                mCommands.insert(std::string(input));
            }
            renderer.preFrame();
            renderer.draw("block", {SCREEN_WIDTH, SCREEN_HEIGHT}, {100, 100}, 0, {1, 0.5, 0.5});
            renderer.postFrame();
        }
    }
    return 0;
}
