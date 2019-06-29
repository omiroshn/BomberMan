
#include <Gui/Gui.h>
#include "ResourceManagement/ResourceManager.hpp"
#include "ResourceManagement/Texture.hpp"
#include "GL/glew.h"
#include "Game.hpp"

Gui::Gui()
{

}

Gui::~Gui()
{

}

void Gui::ShowMainMenu()
{
	glViewport(0, 0, mWidth, mHeight);
	ImGui::SetNextWindowSize({mWidth, mHeight}, ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowPos({0, 0},0);
	ImGui::SetNextWindowBgAlpha(1);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(220,100));
	ImGui::SetNextWindowCollapsed(0);

	SetBackground("face");

	ImGui::Begin("Main Menu");
	//ImGui::Image(mBackground,{200,200}, {1,1}, {0,0});
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 20));
	ImGui::Text("Welcome to BomberMan game!");

	/////////////////////////////////HARDNESS////////////////////////////////////

	//ImGui::Separator();

	//ShowHardnessRadioButtons();

	/////////////////////////////////START GAME////////////////////////////////////

	const ImVec2 menu_frame = {200, 158};
	const float spacing = 10;
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, spacing));
	ImGui::BeginChildFrame(1, menu_frame, 4);

	ShowStartNewGameMenu();

	/////////////////////////////////LOAD GAME////////////////////////////////////

	ShowLoadSavedGamesMenu();

	/////////////////////////////////SETTINGS OF GAME////////////////////////////////////

	ShowSettingsMenu();

	/////////////////////////////////EXIT////////////////////////////////////

	if (ImGui::Button("EXIT", STANDARD_MENU_BUTTON))
	{
		Game::mIsRunning = false;
	}

	ImGui::EndChildFrame();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::End();
	ImGui::PopStyleVar();
}

void Gui::ShowInGameMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Settings"))
		{
			if (ImGui::MenuItem("Open Popup"))
			{
				ImGui::OpenPopup("Some Popup");
			}
			ImGui::EndMenu();
		}
		ImGui::Text("Your score is:");
		ImGui::EndMainMenuBar();
	}

	if (ImGui::BeginPopup("Some Popup"))
	{
		ImGui::Text("Hello World");
		ImGui::EndPopup();
	}
}

void Gui::ShowStartNewGameMenu()
{
	if (ImGui::BeginPopup("Select stage"))
	{
		ImGui::BeginChildFrame(2, {200, 204}, 4);
		ImGui::Text("     Choose campaign");
		// static char str0[128] = "Your name";
		// ImGui::InputText("", str0, IM_ARRAYSIZE(str0));
		ImTextureID b1 = (ImTextureID)RESOURCES.getTexture("face")->getTextureID();
		ImTextureID b2 = (ImTextureID)RESOURCES.getTexture("flame-fire")->getTextureID();
		ImTextureID b3 = (ImTextureID)RESOURCES.getTexture("cloud_trans")->getTextureID();
		ImTextureID b4 = (ImTextureID)RESOURCES.getTexture("explosion_tmap_2")->getTextureID();
		if (ImGui::ImageButton(b1, ImVec2(32, 32), ImVec2(0, 0), ImVec2(32.0f, 32.0f), 2, ImColor(0, 0, 0, 255)))
		{
			Game::mChosenStage = 1;
			GamePaused(false);
			StartTheGame(true);
		}
		ImGui::SameLine();
		ImGui::Text("\nStage 1");
		if (ImGui::ImageButton(b2, ImVec2(32, 32), ImVec2(0, 0), ImVec2(32.0f, 32.0f), 2, ImColor(0, 0, 0, 255)))
		{
			Game::mChosenStage = 2;
			GamePaused(false);
			StartTheGame(true);
		}
		ImGui::SameLine();
		ImGui::Text("\nStage 2");
		if (ImGui::ImageButton(b3, ImVec2(32, 32), ImVec2(0, 0), ImVec2(32.0f, 32.0f), 2, ImColor(0, 0, 0, 255)))
		{
			Game::mChosenStage = 3;
			GamePaused(false);
			StartTheGame(true);
		}
		ImGui::SameLine();
		ImGui::Text("\nStage 3");
		if (ImGui::ImageButton(b4, ImVec2(32, 32), ImVec2(0, 0), ImVec2(32.0f, 32.0f), 2, ImColor(0, 0, 0, 255)))
		{
			Game::mChosenStage = -1;
			GamePaused(false);
			StartTheGame(true);
		}
		ImGui::SameLine();
		ImGui::Text("\nBonus level");
		ImGui::EndChildFrame();
		// if (ImGui::Button("PLAY", STANDARD_MENU_BUTTON))
		// {
		// 	GamePaused(false);
		// 	StartTheGame(true);
		// 	return;
		// }
		ImGui::EndPopup();
	}
	if (ImGui::Button("Start new GAME", STANDARD_MENU_BUTTON))
	{
		ImGui::OpenPopup("Select stage");
		

	}
}

void Gui::ShowLoadSavedGamesMenu()
{
	if (ImGui::BeginPopup("Continue"))
	{
		const ImVec2 saved_frame = {200, 260};
		ImGui::BeginChildFrame(3, saved_frame, 4);
		ImGui::Columns(2);
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Players name");
		ImGui::NextColumn();
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Score");
		ImGui::NextColumn();

		ImGui::Separator();

		ImGui::EndChildFrame();
		ImGui::Button("PLAY", STANDARD_MENU_BUTTON);
		ImGui::EndPopup();
	}

	if (ImGui::Button("Load Game", STANDARD_MENU_BUTTON))
	{
		ImGui::OpenPopup("Saved Games");
	}
}

void Gui::ShowSettingsMenu()
{
	if (ImGui::BeginPopup("Settings of the Games"))
	{
		const ImVec2 saved_frame = {200, 320};
		ImGui::BeginChildFrame(3, saved_frame, 4);

		ImGui::Text("\nSet music volume\n");
		ImGui::SliderInt("10", &Game::mMusicVolume, 0, 10, "Music");
		ImGui::Separator();

		ImGui::Text("\nSet sounds volume\n");
		ImGui::SliderInt("9", &Game::mSoundsVolume, 0, 9, "Sounds");
		ImGui::Separator();

		ImGui::Text("\nSet keybinding\n");
		ImGui::RadioButton("Arrows", &Game::mKeyBindVolume, 0);ImGui::SameLine();
		ImGui::RadioButton("ASWD", &Game::mKeyBindVolume, 1);ImGui::SameLine();
		ImGui::RadioButton("HJKL", &Game::mKeyBindVolume, 2);
		ImGui::Separator();

		ImGui::Checkbox("FullScreen", &Game::mWindowed);
		
		ImGui::Text("\nSet screen resolution\n");
         const char* items[] = {"360", "480", "720", "1400"};
		//ImGui::Combo("combo", &Game::mScreenResolution, "360\0res480\0res720\0res1000\0res1200\0\0");
		ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;

		ImGuiStyle& style = ImGui::GetStyle();
		float w = ImGui::CalcItemWidth();
		float spacing = style.ItemInnerSpacing.x;
		float button_sz = ImGui::GetFrameHeight();
		ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
		if (ImGui::ArrowButton("##r", ImGuiDir_Left))
		{
			if (Game::mScreenResolution > 0)
				Game::mScreenResolution--;
		}
		static const char* current_item = items[Game::mScreenResolution];
		ImGui::SameLine(0, spacing);
		if (ImGui::BeginCombo("##custom combo", current_item, ImGuiComboFlags_NoArrowButton))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				bool is_selected = (current_item == items[n]);
				if (ImGui::Selectable(items[n], is_selected))
					current_item = items[n];
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine(0, spacing);
		if (ImGui::ArrowButton("##r", ImGuiDir_Right))
		{
			if (Game::mScreenResolution < 4)
				Game::mScreenResolution++;
		}

		ImGui::EndChildFrame();
		if (ImGui::Button("CONTINUE", STANDARD_MENU_BUTTON))
		{
			GamePaused(false);
			ImGui::EndPopup();
			StartTheGame(true);
			return;
		}
		ImGui::EndPopup();

	}
	if (ImGui::Button("Settings", STANDARD_MENU_BUTTON))
	{
		GamePaused(true);
		ImGui::OpenPopup("Settings of the Games");
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
}

void Gui::SetBackground(const char* texture)
{
	mBackground = (ImTextureID)RESOURCES.getTexture(texture)->getTextureID();
}


void Gui::ShowHardnessRadioButtons()
{
	static int hardness = 0;
	ImGui::RadioButton("Easy", &hardness, 0);ImGui::SameLine();

	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("For beginners");

	ImGui::RadioButton("Middle", &hardness,1);ImGui::SameLine();

	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Recommended");

	ImGui::RadioButton("Hard", &hardness, 2);

	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Super hard map");
}

void Gui::ChangeMenuSize(int w, int h)
{
	mWidth = w;
	mHeight = h;
}