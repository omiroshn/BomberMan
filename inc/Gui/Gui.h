#ifndef BOMBERMAN_GUI_H
#define BOMBERMAN_GUI_H

#include <imgui.h>
#include <vector>
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

	bool Init(SDL_Window* window);
	void Shutdown();
	void NewFrame(SDL_Window* window);
	void RenderDrawData(ImDrawData* draw_data);

	void InvalidateDeviceObjects();
	bool CreateDeviceObjects();

	static bool sMousePressed[3];

private:
	void ShowStartNewGameMenu();
	void ShowLoadSavedGamesMenu();
	void ShowSettingsMenu();
	void ChangeStage(int next_stage);

	void CreateFontsTexture();

	static const char*	GetClipboardText(void*);
	static void 		SetClipboardText(void*, const char *text);

	bool mGameStarted = false;
	bool mGamePaused = false; //make static in Game
	ImGuiWindowFlags mWindow_flags = 0;
	CurrentMenu mCurrentMenu = CurrentMenu::mainMenu;
	ImTextureID mBackground = nullptr;
	float mWidth = 800.0f;
	float mHeight = 600.0f;
	std::vector<ImTextureID> mButtonsTextures;


// SDL data
Uint64       mTime = 0;
SDL_Cursor*  mMouseCursors[ImGuiMouseCursor_COUNT] = { 0 };

static char*        sClipboardTextData;

// OpenGL data
char         mGlslVersion[32] = {"#version 330\n"};
GLuint       mFontTexture = 0;
int          mAttribLocationTex = 0, mAttribLocationProjMtx = 0;
int          mAttribLocationPosition = 0, mAttribLocationUV = 0, mAttribLocationColor = 0;
unsigned int mVaoHandle = 0, mVboHandle = 0,mElementsHandle = 0;
};

#endif //BOMBERMAN_GUI_H
