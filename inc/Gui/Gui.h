#ifndef BOMBERMAN_GUI_H
#define BOMBERMAN_GUI_H

#include <imgui.h>
#include "Gui/imgui_impl_sdl_gl3.h"

#define  WIN_SIZE {800, 600}
#define  STANDARD_MENU_BUTTON {200, 30}

class Gui
{
public:
	Gui();
	~Gui();
	void ShowMainMenu();
	bool IsGameRunning();
	void StartTheGame(bool start);

private:
	void ShowHardnessRadioButtons();
	void ShowStartNewGameMenu();
	void ShowLoadSavedGamesMenu();
	bool mGameStarted = false;
	bool mShowMenu = true;
	bool mShowScore = false;
	int  mMapIndex = -1;
};

#endif //BOMBERMAN_GUI_H
