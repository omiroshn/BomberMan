#include <Gui/Gui.h>
#include "ResourceManagement/ResourceManager.hpp"
#include "ResourceManagement/Texture.hpp"
#include "GL/glew.h"
#include "Game.hpp"
#include  "Configure.hpp"

Gui::Gui()
{
	mWindow_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar;	
}

void Gui::ShowMainMenu()
{
	mWidth = (float)CONFIGURATION.getWidth();
	mHeight = (float)CONFIGURATION.getHeight();
	if (!mBackground)
		mBackground = RESOURCES.getTexture("sky").get();

	ImGui::SetNextWindowPos({0, 0},0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(mWidth, mHeight));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	ImGui::Begin("Main Menu", NULL, mWindow_flags);
	ImGui::GetWindowDrawList()->AddImage(mBackground, ImVec2(0, 0), ImVec2(mWidth, mHeight));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(mWidth / 2 - 112, mHeight / 2 - 200));
	ImGui::BeginChildFrame(5, ImVec2(mWidth, mHeight), ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NavFlattened);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2,2));

	if (mCurrentMenu == CurrentMenu::mainMenu)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 20));
		ImGui::Text("                BomberMan");

		/////////////////////////////////START GAME////////////////////////////////////
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 2));
		ImGui::BeginChildFrame(1, {224, 204}, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NavFlattened);

		ShowStartNewGameMenu();

		/////////////////////////////////LOAD GAME////////////////////////////////////
		if (ImGui::Button("Continue", STANDARD_MENU_BUTTON))
		{
			mCurrentMenu = CurrentMenu::changeStageMenu;
			ShowLoadSavedGamesMenu();
		}

		/////////////////////////////////SETTINGS////////////////////////////////////

		if (ImGui::Button("Settings", STANDARD_MENU_BUTTON))
		{
			GamePaused(true);
			mCurrentMenu = CurrentMenu::settingsMenu;
			ShowSettingsMenu();
		}

		/////////////////////////////////EXIT////////////////////////////////////

		if (ImGui::Button("EXIT", STANDARD_MENU_BUTTON))
		{
			Game::get()->requestExit();
		}

		ImGui::EndChildFrame();		
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}
	else if (mCurrentMenu == CurrentMenu::settingsMenu)
		ShowSettingsMenu();
	else if (mCurrentMenu == CurrentMenu::changeStageMenu)
		ShowLoadSavedGamesMenu();

	ImGui::PopStyleVar();
	ImGui::EndChildFrame();
	ImGui::PopStyleVar();
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void Gui::ShowInGameMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		ImGui::Text("Stage: ");
		std::string stage_to_show = std::to_string(CONFIGURATION.getChosenStage());
		stage_to_show = stage_to_show == "0" ? "BONUS" : stage_to_show;
		ImGui::Text("%s", stage_to_show.c_str());

		ImGui::Text("Score: ");
		ImGui::Text("%s", std::to_string(CONFIGURATION.getScore()).c_str());

		ImGui::Text("   Time: ");
		ImGui::Text("%s", std::to_string(static_cast<int>(Game::mStageTimer)).c_str());

		ImGui::Text("   Lives: ");
		ImGui::Text("%s", std::to_string(CONFIGURATION.getLives()).c_str());

		auto* game = Game::get();
		auto& hero = game->getHero();
		if (hero.mStats.wallpass)
			ImGui::Image(RESOURCES.getTexture("wallpass").get(), {20,20});

		ImGui::EndMainMenuBar();
	}

	if (ImGui::BeginPopup("Some Popup"))
	{
		ImGui::Text("Hello World");
		ImGui::EndPopup();
	}
}

void Gui::ShowBetweenStageScreen()
{
	int stage = CONFIGURATION.getChosenStage();
	switch (stage)
	{
		case 0:
			ShowLoadingScreen("face");
			break;
		case 1:
			ShowLoadingScreen("flame-fire");
			break;
		case 2:
			ShowLoadingScreen("brickwall");
			break;
		case 3:
			ShowLoadingScreen("explosion_tmap_2");
			break;
		case 4:
			ShowLoadingScreen("face");
			break;
		default:
			break;
	}
}

void Gui::ShowStartNewGameMenu()
{
	if (ImGui::Button("Start new Campaign", STANDARD_MENU_BUTTON))
	{
		mCurrentMenu = CurrentMenu::inGameMenu;
		CONFIGURATION.setChosenStage(DefaultChosenStage);
		CONFIGURATION.setLives(DefaultLives);
		CONFIGURATION.setBestLevelAchieved(DefaultBestLevelAchieved);
		CONFIGURATION.setScore(DefaultScore);
		if (IsGameRunning())
			Game::mReloadStage = true;
		GamePaused(false);
		StartTheGame(true);
	}
}

void Gui::ChangeStage(int next_stage)
{
	if (next_stage > CONFIGURATION.getBestLevelAchieved())
			return;
	CONFIGURATION.setChosenStage(next_stage + 1);
	GamePaused(false);
	StartTheGame(true);
	Game::mReloadStage = true;
	if (Game::mStageTimer > 4)
		Game::mStageStartedTimer = Game::getCurrentTime();
	Game::mStageTimer = 3 - (Game::getCurrentTime() - Game::mStageStartedTimer);
}


void Gui::ShowLoadSavedGamesMenu()
{
	std::shared_ptr<Texture> mButtonsTextures[]{
		RESOURCES.getTexture("unlocked0"),
		RESOURCES.getTexture("unlocked1"),
		RESOURCES.getTexture("unlocked2"),
		RESOURCES.getTexture("unlocked3"),
		RESOURCES.getTexture("locked")
	};

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(28, 4));
	ImGui::BeginChildFrame(2, { 234, 204 }, 4 | ImGuiWindowFlags_NavFlattened);
	ImGui::Text("\nStages of the campaign");
	if (ImGui::ImageButton(mButtonsTextures[0].get(), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), 2, ImColor(0, 0, 0, 255)))
	{
		ChangeStage(0);
	}
	ImGui::SameLine();
	ImGui::Text("\n   Stage 1");
	if (ImGui::ImageButton(mButtonsTextures[CONFIGURATION.getBestLevelAchieved() > 0 ? 1 : 4].get(), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), 2, ImColor(0, 0, 0, 255)))
	{
		ChangeStage(1);
	}
	ImGui::SameLine();
	ImGui::Text("\n   Stage 2");
	if (ImGui::ImageButton(mButtonsTextures[CONFIGURATION.getBestLevelAchieved() > 1 ? 2 : 4].get(), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), 2, ImColor(0, 0, 0, 255)))
	{
		ChangeStage(2);
	}
	ImGui::SameLine();
	ImGui::Text("\n   Stage 3");
	if (ImGui::ImageButton(mButtonsTextures[CONFIGURATION.getBestLevelAchieved() > 2 ? 3 : 4].get(), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), 2, ImColor(0, 0, 0, 255)))
	{
		ChangeStage(3);
	}
	ImGui::SameLine();
	ImGui::Text("\n   Bonus level");
	ImGui::EndChildFrame();
	ImGui::PopStyleVar();
	if (ImGui::Button("BACK", { 234, 48 }))
	{
		mCurrentMenu = CurrentMenu::mainMenu;
		return;
	}
}

void Gui::ShowSettingsMenu()
{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(40, 4));
		const ImVec2 saved_frame = {234, 330};
		ImGui::BeginChildFrame(3, saved_frame, 4 | ImGuiWindowFlags_NavFlattened);

		ImGui::Text("\n    Set music volume\n");
		ImGui::SliderInt("1O", &CONFIGURATION.getMusicVolume(), 0, 10, "Music");
		ImGui::Separator();

		ImGui::Text("\n   Set sounds volume\n");
		ImGui::SliderInt("10", &CONFIGURATION.getSoundVolume(), 0, 10, "Sounds");
		ImGui::Separator();

		ImGui::Text("\n    Set keybinding\n");
		ImGui::RadioButton("Arrows", &CONFIGURATION.getKeyBindVolume(), 0);ImGui::SameLine();
		ImGui::RadioButton("ASWD", &CONFIGURATION.getKeyBindVolume(), 1);
		ImGui::RadioButton("HJKL", &CONFIGURATION.getKeyBindVolume(), 2);
		ImGui::Separator();

		if (ImGui::Checkbox("     FullScreen", &CONFIGURATION.getWindowed()))
		{
			CONFIGURATION.setWindowed(CONFIGURATION.getWindowed());
		}

		ImGui::Text("\nSet screen resolution\n");
        const char* items[] = {"720 p", "1080 p", "1200 p", "1440 p"};
		glm::ivec2 resolutions[] = {{960, 720},{1280, 1080},{1600, 1200},{1920, 1440}};
		ImGuiStyle& style = ImGui::GetStyle();
		float w = ImGui::CalcItemWidth();
		float spacing = style.ItemInnerSpacing.x;
		float button_sz = ImGui::GetFrameHeight();
		ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
		if (ImGui::ArrowButton("##l", ImGuiDir_Left))
		{
			if (CONFIGURATION.getWidth() >= 1280)
			{
				CONFIGURATION.getScreenResolution()--;
				CONFIGURATION.setSize(resolutions[CONFIGURATION.getScreenResolution()][0],resolutions[CONFIGURATION.getScreenResolution()][1]);
			}
		}
		static const char* current_item = items[CONFIGURATION.getScreenResolution()];
		ImGui::SameLine(0, spacing);
		if (ImGui::BeginCombo("##custom combo", current_item, ImGuiComboFlags_NoArrowButton))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				bool is_selected = (current_item == items[n]);
				if (ImGui::Selectable(items[n], is_selected))
				{
					CONFIGURATION.setSize(resolutions[n][0],resolutions[n][1]);
					current_item = items[n];
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine(0, spacing);
		if (ImGui::ArrowButton("##r", ImGuiDir_Right))
		{
			if (CONFIGURATION.getWidth() <= 1200)
			{
				CONFIGURATION.getScreenResolution()++;
				CONFIGURATION.setSize(resolutions[CONFIGURATION.getScreenResolution()][0],resolutions[CONFIGURATION.getScreenResolution()][1]);
			}
		}
		ImGui::EndChildFrame();
		ImGui::PopStyleVar();

		if (ImGui::Button("BACK", {234, 48}))
		{
			mCurrentMenu = CurrentMenu::mainMenu;
			return;
		}

}

bool Gui::IsGameRunning()
{
	return mGameStarted && !mGamePaused;
}

void Gui::StartTheGame(bool start)
{
	mGameStarted = start;
}

void Gui::GamePaused(bool state)
{
	mGamePaused = state;
	if (!state)
		mCurrentMenu = CurrentMenu::mainMenu;
}

void Gui::SetBackground(const char* texture)
{
	mBackground = RESOURCES.getTexture(texture).get();
}

void Gui::ShowLoadingScreen(const char* screen)
{
	ImTextureID im = RESOURCES.getTexture(screen).get();
	ImGui::SetNextWindowPos({0, 0},0);
	mWidth = (float)CONFIGURATION.getWidth();
	mHeight = (float)CONFIGURATION.getHeight();
	ImGui::SetNextWindowSize({mWidth,mHeight});
	ImGui::Begin("Next Stage", NULL, mWindow_flags);
	ImGui::Image(im,{mWidth,mHeight}, {0,0}, {1,1});
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 20));
	if (CONFIGURATION.getChosenStage() > 0)
	{
		char stage[] = "Stage:  ";
		stage[7] = CONFIGURATION.getChosenStage() + '0';
		ImGui::GetWindowDrawList()->AddText( ImVec2(mWidth / 2 - 20, mHeight / 2 + 30), ImColor(1.0f,1.0f,1.0f,1.0f), stage);
	}
	else
	{
		char stage[] = "Stage:  Bonus Level";
		ImGui::GetWindowDrawList()->AddText( ImVec2(mWidth / 2 - 20, mHeight / 2 + 30), ImColor(1.0f,1.0f,1.0f,1.0f), stage);
	}
	ImGui::PopStyleVar();
	ImGui::End();
}

Gui::~Gui()
{

}

void Gui::RenderDrawData(ImDrawData* draw_data)
{
    ImGuiIO& io = ImGui::GetIO();
    int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    draw_data->ScaleClipRects(io.DisplayFramebufferScale);
    glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);

    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_SCISSOR_TEST);

	glm::mat4 orthoProj = glm::ortho(0.f, io.DisplaySize.x, io.DisplaySize.y, 0.f);

	static auto guiShader = RESOURCES.getShader("gui");
	guiShader->use();
	guiShader->setMat4("ProjMtx", orthoProj);

    glBindVertexArray(mVaoHandle);

    // Draw
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawIdx* idx_buffer_offset = 0;

        glBindBuffer(GL_ARRAY_BUFFER, mVboHandle);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementsHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->TextureId)
				reinterpret_cast<Texture *>(pcmd->TextureId)->bind();
			glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
			glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer_offset);
            idx_buffer_offset += pcmd->ElemCount;
        }
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST); 
    glDisable(GL_SCISSOR_TEST);
}

void Gui::CreateFontsTexture()
{
    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	mFontTexture = RESOURCES.loadTextureFromMemory(pixels, TextureType::Font, width, height, 4, false);

    io.Fonts->TexID = mFontTexture.get();
}

void Gui::CreateDeviceObjects()
{
	static auto guiShader = RESOURCES.getShader("gui");
	guiShader->setInt("Texture", 0);

    glGenBuffers(1, &mVboHandle);
    glGenBuffers(1, &mElementsHandle);

    glGenVertexArrays(1, &mVaoHandle);
    glBindVertexArray(mVaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, mVboHandle);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));

    CreateFontsTexture();
}

void    Gui::InvalidateDeviceObjects()
{
    if (mVboHandle)
		glDeleteBuffers(1, &mVboHandle);
    if (mElementsHandle)
		glDeleteBuffers(1, &mElementsHandle);
    mVboHandle = mElementsHandle = 0;

    if (mFontTexture)
    {
        ImGui::GetIO().Fonts->TexID = 0;
        mFontTexture.reset();
    }
}

bool    Gui::Init()
{
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
    io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
    io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
    io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
    io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
    io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
    io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
    io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
    io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
    io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
    io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
    io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
    io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;

    io.ClipboardUserData = NULL;

    return true;
}

void Gui::Shutdown()
{
    // Destroy OpenGL objects
    Gui::InvalidateDeviceObjects();
}

void Gui::NewFrame(SDL_Window* window)
{
    if (!mFontTexture)
        CreateDeviceObjects();

    ImGuiIO& io = ImGui::GetIO();

    // Setup display size (every frame to accommodate for window resizing)
    int w, h;
    int display_w, display_h;
    SDL_GetWindowSize(window, &w, &h);
    SDL_GL_GetDrawableSize(window, &display_w, &display_h);
    io.DisplaySize = ImVec2((float)w, (float)h);
    io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);
	io.DeltaTime = Game::mDeltaTime;

    int mx, my;
    Uint32 mouse_buttons = SDL_GetMouseState(&mx, &my);
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
    io.MouseDown[0] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
    io.MouseDown[1] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
    io.MouseDown[2] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;

    if ((SDL_GetWindowFlags(window) & (SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_MOUSE_CAPTURE)) != 0)
        io.MousePos = ImVec2((float)mx, (float)my);
    ImGui::NewFrame();
}
