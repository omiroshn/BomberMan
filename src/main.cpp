#include "Core.hpp"

//This must be done for proper work of SDL2 lib
#undef main

#include "ResourceManagement/ResourceManager.hpp"
#include "Game.hpp"
#include "ResourceManagement/FfmpegPlayer.hpp"

extern "C" {
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavformat/avio.h"
    #include "libavutil/file.h"
	#include <libswscale/swscale.h>
}

int main(int ac, char **av)
{
    std::string exePath(av[0]);
    std::string exeFolder(exePath.substr(0, exePath.find_last_of("\\/") + 1));
    RESOURCES.setBinFolder(exeFolder);
	av_register_all();
	FFMPEG.setBinFolder(exeFolder);

    Game game;

    game.start();

    return 0;
}
