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
    SDL_Event & getEvent();
    SDL_Joystick *getJoystick();
	void tickGui();
    void getSize(int &w, int &h);
    std::vector<int> getSize();
    bool IsGameRunning();
    void ShowStartingMenu();
    void ShowInGameMenu();
    void ShowBetweenStageScreen();
    void PauseGame(bool state);
	void setSize(int const w, int const h);
	void setFullScreen(bool isFullScreen);

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
    SDL_Joystick *mJoystick;
	Gui          *mMainMenu;
};
