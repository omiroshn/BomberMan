#include "Core.h"
#include <iostream>
#include <unordered_set>
#include <vector>

#undef main

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define TESTRENDERER


#ifndef TESTRENDERER
#include "Renderer.hpp"

std::unordered_set<std::string> mCommands;
int main(int ac, char **av) {
    bool isRunning = true;
    {
        std::string exePath(av[0]);
        std::string exeFolder(exePath.substr(0, exePath.find_last_of("\\/")));
        Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT, "Prototype", exeFolder);
        while (isRunning) {
            std::string input = renderer.processInput(isRunning);
            if (!input.empty()) {
                mCommands.insert(std::string(input));
            }
            renderer.preFrame();
            renderer.draw("block", {SCREEN_WIDTH, SCREEN_HEIGHT}, {100, 100}, 0, {1, 0.5, 0.5});
            renderer.postFrame();
        }
    }
        return 0;
}
#else
#include "TestRenderer.hpp"
#include "NewResourceManager.hpp"

std::unordered_set<std::string> mCommands;
int main(int ac, char **av) {
    bool isRunning = true;
    {
        TestRenderer t(SCREEN_WIDTH, SCREEN_HEIGHT, "Prototype");

        std::string exePath(av[0]);
        std::string exeFolder(exePath.substr(0, exePath.find_last_of("\\/") + 1));
        NewResourceManager::getInstance().setBinFolder(exeFolder);
        NewResourceManager::getInstance().loadShader("model_core.vx.glsl", "model_core.ft.glsl", "modelShader");
        NewResourceManager::getInstance().loadShader("sprite.vx.glsl", "sprite.ft.glsl", "sprite");
        NewResourceManager::getInstance().loadTexture("block.png", "block", false);

        t.setProjection();
        while (isRunning)
        {
            std::string input = t.processInput(isRunning);
            if (!input.empty())
            {
                mCommands.insert(std::string(input));
            }
            t.preFrame();
            t.draw("block", {SCREEN_WIDTH, SCREEN_HEIGHT}, {100, 100}, 0, {1, 0.5, 0.5});
            t.postFrame();
        }
    }
    return 0;
}
#endif
