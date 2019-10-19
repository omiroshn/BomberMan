#pragma once
#include <string>
#include <GL/glew.h>
#include <SDL.h>
#include <Gui/Gui.h>

class GameWindow
{
public:
    GameWindow(int width, int height, std::string const &winName);
    ~GameWindow();
    void update();
	void tickGui();
    void getSize(int &w, int &h);
	SDL_Window   *getSDLWindow() const;
	SDL_GLContext const & getSDLGLContext() const;
	void getPosition(int &x, int &y);
	void setPosition(int const x, int const y);
    bool IsGameRunning();
    void ShowStartingMenu();
    void ShowInGameMenu();
    void ShowBetweenStageScreen();
    void PauseGame(bool state);
	void setSize(int const w, int const h);
	void setFullScreen(bool isFullScreen);
	GameWindow(GameWindow const&) = delete;
    GameWindow& operator=(GameWindow const&) = delete;

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
	Gui           mMainMenu;
};
