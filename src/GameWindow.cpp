
#include "CustomException.hpp"
#include <iostream>
#include <GameWindow.hpp>
#include "Core.hpp"
#include "ResourceManagement/ResourceManager.hpp"

GameWindow::GameWindow(int aWidth, int aHeight, std::string const &aWinName) :
    mWidth(aWidth), mHeight(aHeight), mName(aWinName)
{
    initWindow();
}

GameWindow::~GameWindow()
{
	mMainMenu.Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(mContext);

    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void GameWindow::initWindow()
{
    initSDL();

    mWindow = SDL_CreateWindow(mName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWidth, mHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    mContext = SDL_GL_CreateContext(mWindow);
    SDL_GL_MakeCurrent(mWindow, mContext);
    SDL_SetWindowResizable(mWindow, SDL_TRUE);
    initOpenGL();

    initGui();
}

void GameWindow::initSDL()
{
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO);

    int context_flags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
#if (__APPLE__)
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
#else
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
#endif
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

void GameWindow::initGui()
{

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    auto fontData = RESOURCES.loadFont("future.ttf");
	ImFontConfig config;
	config.FontDataOwnedByAtlas = false;
	io.Fonts->AddFontFromMemoryTTF(fontData.data(), static_cast<int>(fontData.size()), 16, &config);

    io.IniFilename = NULL;
    io.LogFilename = NULL;
    io.WantSaveIniSettings = false;
    io.WantTextInput = false;

    ImGui::StyleColorsDark();

	mMainMenu.Init();
}

void GameWindow::initOpenGL()
{
	glewExperimental = true;
    GLenum err = glewInit();
	if (err != GLEW_OK)
    {
		BM_CRITICAL("%s", glewGetErrorString(err));
        exit(0);
    }
    glViewport(0, 0, mWidth, mHeight);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glEnable(GL_MULTISAMPLE);
}

void GameWindow::update()
{
    ImGui::EndFrame();
    ImGui::Render();
	mMainMenu.RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(mWindow);
}

void GameWindow::tickGui()
{
	mMainMenu.NewFrame(mWindow);
}

void GameWindow::getSize(int &w, int &h)
{
    SDL_GetWindowSize(mWindow, &w, &h);
}

bool GameWindow::IsGameRunning() {
    return mMainMenu.IsGameRunning();
}

void GameWindow::ShowStartingMenu()
{
    mMainMenu.ShowMainMenu();
}

void GameWindow::ShowInGameMenu()
{
    mMainMenu.ShowInGameMenu();
}

void GameWindow::ShowBetweenStageScreen()
{
    mMainMenu.ShowBetweenStageScreen();
}

void GameWindow::PauseGame(bool state)
{
    mMainMenu.GamePaused(state);
}

void GameWindow::setSize(int const w, int const h)
{
    SDL_SetWindowSize(mWindow, w, h);
}

void 		GameWindow::setPosition(int const x, int const y)
{
	SDL_SetWindowPosition(mWindow, x, y);
}

void GameWindow::setFullScreen(bool isFullScreen)
{
	if (isFullScreen)
		SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else
		SDL_SetWindowFullscreen(mWindow, 0);
}


void GameWindow::getPosition(int &x, int &y)
{
    SDL_GetWindowPosition(mWindow, &x, &y);
}

SDL_Window   *GameWindow::getSDLWindow() const
{
	return mWindow;
}

SDL_GLContext const & GameWindow::getSDLGLContext() const
{
	return mContext;
}
