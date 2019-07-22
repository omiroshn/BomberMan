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

	bool ImGui_Init(SDL_Window* window);
	void ImGui_Shutdown();
	void ImGui_NewFrame(SDL_Window* window);
	void ImGui_RenderDrawData(ImDrawData* draw_data);

	void ImGui_InvalidateDeviceObjects();
	bool ImGui_CreateDeviceObjects();

	static bool sMousePressed[3];

private:
	void ShowStartNewGameMenu();
	void ShowLoadSavedGamesMenu();
	void ShowSettingsMenu();
	void ChangeStage(int next_stage);

	void ImGui_CreateFontsTexture();

	static const char* ImGui_GetClipboardText(void*);
	static void 		ImGui_SetClipboardText(void*, const char *text);

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
char         mGlslVersion[32] = {"#version 330"};
GLuint       mFontTexture = 0;
int          mShaderHandle = 0, mVertHandle = 0, mFragHandle = 0;
int          mAttribLocationTex = 0, mAttribLocationProjMtx = 0;
int          mAttribLocationPosition = 0, mAttribLocationUV = 0, mAttribLocationColor = 0;
unsigned int mVboHandle = 0,mElementsHandle = 0;
};

#endif //BOMBERMAN_GUI_H
