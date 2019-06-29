
#include "CustomException.hpp"
#include <iostream>
#include <GameWindow.hpp>
#include "Core.hpp"

GameWindow::GameWindow(int aWidth, int aHeight, std::string const &aWinName) :
    mWidth(aWidth), mHeight(aHeight), mName(aWinName)
{
    initWindow();
}


GameWindow::~GameWindow()
{
    ImGui_ImplSdlGL3_Shutdown();
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
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
    
    int context_flags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);


    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


    //SDL_SetRelativeMouseMode(SDL_TRUE);
}

void GameWindow::initGui() {

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    ImGui_ImplSdlGL3_Init(mWindow);

    ImGui::StyleColorsDark();

	mMainMenu = new Gui();
    // io.Fonts->AddFontFromFileTTF("Assets/font/Roboto-Medium.ttf", 16.0f);

}

void GameWindow::initOpenGL()
{
	glewExperimental = true;
    GLenum err = glewInit();
	if (err != GLEW_OK)
    {
		BM_CRITICAL("%s", glewGetErrorString(err));
        abort();
    }
    glViewport(0, 0, mWidth, mHeight);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
}

void GameWindow::update()
{
	// if (!IsGameRunning())
	//     {
    //         mMainMenu->ShowMainMenu();
    //     }
    ImGui::Render();
    ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(mWindow);

    SDL_PollEvent(&mEvent);
}

SDL_Event & GameWindow::getEvent()
{
    return mEvent;
}

void GameWindow::tickGui()
{
    ImGui_ImplSdlGL3_NewFrame(mWindow);
}

void GameWindow::getSize(int &w, int &h)
{
    SDL_GetWindowSize(mWindow, &w, &h);
}

bool GameWindow::IsGameRunning() {
    return mMainMenu->IsGameRunning();
}

void GameWindow::ShowStartingMenu()
{
    mMainMenu->ShowMainMenu();
}

void GameWindow::ShowInGameMenu()
{
    mMainMenu->ShowInGameMenu();
}

void GameWindow::PauseGame(bool state)
{
    mMainMenu->GamePaused(state);
    mMainMenu->ShowMainMenu();
}