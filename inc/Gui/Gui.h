#ifndef BOMBERMAN_GUI_H
#define BOMBERMAN_GUI_H

#include <imgui.h>
#include "Gui/imgui_impl_sdl_gl3.h"
#include "StateMachine/StateMachine.h"

#define  STANDARD_MENU_BUTTON {200, 30}

class Gui
{
public:
	Gui();
	~Gui();
	void ShowMainMenu();
	void ShowInGameMenu();
	bool IsGameRunning();
	void StartTheGame(bool start);
	void GamePaused(bool state);
	void SetBackground(const char* texture);
	void ChangeMenuSize(int w, int h);
	void ShowLoadingScreen(const char* screen);

private:
	void ShowHardnessRadioButtons();
	void ShowStartNewGameMenu();
	void ShowLoadSavedGamesMenu();
	void ShowSettingsMenu();
	bool mGameStarted = false;
	bool mGamePaused = false; //make static in Game
	bool mShowMenu = true;
	bool mShowScore = false;
	int  mMapIndex = -1;
	ImTextureID mBackground = nullptr;
	float mWidth = 800.0f;
	float mHeight = 600.0f;
	std::vector<ImTextureID> mButtonsTextures;
};

#endif //BOMBERMAN_GUI_H
