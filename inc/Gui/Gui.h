/*!
\file
\brief Header file of the Gui class

This file contains declaration of the Gui class.
*/
#ifndef BOMBERMAN_GUI_H
#define BOMBERMAN_GUI_H

#include <imgui.h>
#include <vector>
#include <memory>
#include <ResourceManagement/Texture.hpp>
#include "SDL.h"
#include "GL/glew.h"


#define  STANDARD_MENU_BUTTON {220, 48}

enum class CurrentMenu
{
	mainMenu,
	settingsMenu,
	changeStageMenu,
	exitMenu,
	inGameMenu
};
/*!
\brief The class to manage user interface

This class suppose to create all GUI instances and manage mouse input on it
*/
class Gui
{
public:
	Gui();
	~Gui();
	Gui(Gui const&) = delete;
    Gui& operator=(Gui const&) = delete;
	void ShowMainMenu();
	void ShowInGameMenu();
    void ShowBetweenStageScreen();
	void ShowLoadingScreen(const char* screen);
	bool IsGameRunning();
	void StartTheGame(bool start);
	void GamePaused(bool state);
	void SetBackground(const char* texture);

	bool Init();
	void Shutdown();
	void NewFrame(SDL_Window* window);
	void RenderDrawData(ImDrawData* draw_data);

	void InvalidateDeviceObjects();
	void CreateDeviceObjects();


private:
	void ShowStartNewGameMenu();
	void ShowLoadSavedGamesMenu();
	void ShowSettingsMenu();
	void ChangeStage(int next_stage);

	void CreateFontsTexture();

	bool mGameStarted = false;
	bool mGamePaused = false;
	ImGuiWindowFlags mWindow_flags = 0;
	CurrentMenu mCurrentMenu = CurrentMenu::mainMenu;
	ImTextureID mBackground = nullptr;
	float mWidth = 800.0f;
	float mHeight = 600.0f;

	GLuint       mVaoHandle = 0, mVboHandle = 0, mElementsHandle = 0;

	std::shared_ptr<Texture> mFontTexture = 0;
};

#endif //BOMBERMAN_GUI_H
