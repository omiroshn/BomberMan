#include "Core.h"
#include <iostream>
#include <unordered_set>
#include <vector>
#include "Renderer.hpp"
#undef main

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

std::unordered_set<std::string> mCommands;

int main(int argc, char* args[])
{
  bool isRunning = true;
  Drawer drawer(SCREEN_WIDTH, SCREEN_HEIGHT, "Prototype");
  while (isRunning)
  {
    std::string input = drawer.processInput(isRunning);
    if (!input.empty())
    {
      mCommands.insert(std::string(input));
    }
    drawer.preFrame();
    drawer.draw("block", {SCREEN_WIDTH, SCREEN_HEIGHT}, {100, 100}, 0, {1,0.5,0.5});
    drawer.postFrame();
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}