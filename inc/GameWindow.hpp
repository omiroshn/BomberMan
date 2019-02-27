#pragma once
#include <string>
#include <GL/glew.h>
#include <SDL.h>
#include <Gui/Gui.h>
#include "Core.hpp"
#include "LogicCore/LoadMap.h"

class GameWindow
{
public:
    GameWindow(int width, int height, std::string const &winName);
    ~GameWindow();
    void update();
    SDL_Event const& getEvent();
	void tickGui();

private:
    void initWindow();
    void initSDL();
	void initGui();
    void initOpenGL();

private:
    int           mWidth, mHeight;
    std::string   mName;
    SDL_Window   *mWindow;
    SDL_GLContext mContext;
    SDL_Event     mEvent;
	Gui          *mMainMenu;
};

