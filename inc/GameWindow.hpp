#pragma once
#include <string>
#include <GL/glew.h>
#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl_gl3.h>

#include "Core.hpp"

class GameWindow
{
public:
    GameWindow(int width, int height, std::string const &winName);
    ~GameWindow();
    void update();
    SDL_Event const& getEvent();

private:
    void initWindow();
    void initSDL();
	void initGui();
    void initOpenGL();

private:
    int mWidth, mHeight;
    std::string mName;
    SDL_Window *mWindow;
    SDL_GLContext mContext;
    SDL_Event     mEvent;
};

