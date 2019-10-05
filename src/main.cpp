/*!
\file
\brief Entry point

This file contains the entry point "main" of the program.
*/
#include "Core.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "ResourceManagement/MusicPlayer.hpp"
#include "ResourceManagement/FfmpegPlayer.hpp"
#include "Game.hpp"

int main(int, char **av)
{
    std::string exePath(av[0]);
    std::string exeFolder(exePath.substr(0, exePath.find_last_of("\\/") + 1));
    RESOURCES.setBinFolder(exeFolder);
	MUSIC_PLAYER.setBinFolder(exeFolder);
	FFMPEG.setBinFolder(exeFolder);

    Game game;
    game.start();

    return 0;
}
