#include "Core.h"
#include <iostream>
#include <unordered_set>
#include <vector>
#include "Renderer.hpp"
#undef main

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

std::unordered_set<std::string> mCommands;

int main(int ac, char **av)
{
    bool isRunning = true;
    Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT, "Prototype");
    while (isRunning)
    {
        std::string input = renderer.processInput(isRunning);
        if (!input.empty())
        {
            mCommands.insert(std::string(input));
        }
        renderer.preFrame();
        renderer.draw("block", {SCREEN_WIDTH, SCREEN_HEIGHT}, {100, 100}, 0, {1,0.5,0.5});
        renderer.postFrame();
    }
    return 0;
}