#ifndef BOMBERMAN_GUI_H
#define BOMBERMAN_GUI_H

#include <imgui.h>
#include <vector>
#include <SDL2/SDL.h>

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
	bool ImGui_ProcessEvent(SDL_Event* event);

	void ImGui_InvalidateDeviceObjects();
	bool ImGui_CreateDeviceObjects();


private:
	void ShowStartNewGameMenu();
	void ShowLoadSavedGamesMenu();
	void ShowSettingsMenu();
	void ChangeStage(int next_stage);
	void 		ImGui_CreateFontsTexture();

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
Uint64       g_Time = 0;
bool         g_MousePressed[3] = { false, false, false };
SDL_Cursor*  g_MouseCursors[ImGuiMouseCursor_COUNT] = { 0 };

static char*        g_ClipboardTextData;

// OpenGL data
char         g_GlslVersion[32]{"#version 330"};
GLuint       g_FontTexture = 0;
int          g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
int          g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
int          g_AttribLocationPosition = 0, g_AttribLocationUV = 0, g_AttribLocationColor = 0;
unsigned int g_VboHandle = 0,g_ElementsHandle = 0;
};

#endif //BOMBERMAN_GUI_H
