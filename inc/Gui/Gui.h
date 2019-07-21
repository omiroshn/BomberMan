#ifndef BOMBERMAN_GUI_H
#define BOMBERMAN_GUI_H

#include <imgui.h>
#include <vector>

#define  STANDARD_MENU_BUTTON {220, 48}

enum class CurrentMenu
{
	mainMenu,
	settingsMenu,
	changeStageMenu,
	exitMenu,
	inGameMenu
};

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

private:
	void ShowStartNewGameMenu();
	void ShowLoadSavedGamesMenu();
	void ShowSettingsMenu();
	void ChangeStage(int next_stage);
	bool mGameStarted = false;
	bool mGamePaused = false; //make static in Game
	ImGuiWindowFlags mWindow_flags = 0;
	CurrentMenu mCurrentMenu = CurrentMenu::mainMenu;
	bool mShowScore = false;
	int  mMapIndex = -1;
	ImTextureID mBackground = nullptr;
	float mWidth = 800.0f;
	float mHeight = 600.0f;
	std::vector<ImTextureID> mButtonsTextures;
};

#endif //BOMBERMAN_GUI_H
