#include "Core.hpp"

//This must be done for proper work of SDL2 lib 
#undef main

#include "ResourceManagement/ResourceManager.hpp"
#include "Game.hpp"

int main(int ac, char **av)
{

    std::string exePath(av[0]);
    std::string exeFolder(exePath.substr(0, exePath.find_last_of("\\/") + 1));
    RESOURCES.setBinFolder(exeFolder);

    Game game;

    game.start();

    return 0;
}
