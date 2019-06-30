
#include <Gui/Gui.h>
#include "ResourceManagement/ResourceManager.hpp"
#include "ResourceManagement/Texture.hpp"
#include "GL/glew.h"
#include "Game.hpp"
#include  "Configure.hpp"

//bool isWindowed = CONFIGURATION.getWindowed();

Gui::Gui()
{

}

Gui::~Gui()
{

}

void Gui::ShowMainMenu()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar ;
	mWidth = CONFIGURATION.getWidth();
	mHeight = CONFIGURATION.getHeight();
	
	ImGui::SetNextWindowPos({0, 0},0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(mWidth / 2 - 100, mHeight / 2 - 79));
	ImGui::SetNextWindowCollapsed(0);

	ImGui::Begin("Main Menu", NULL,window_flags);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 20));
	ImGui::Text("Welcome to BomberMan game!");

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
		ImGui::Text("Score: ");
		ImGui::Text("%s", std::to_string(CONFIGURATION.getScore()).c_str());

		ImGui::Text("   Time: ");
		ImGui::Text("%s", std::to_string(Game::mStageTimer).c_str());

		ImGui::Text("   Lives: ");
		ImGui::Text("%s", std::to_string(CONFIGURATION.getLives()).c_str());
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
		//ShowHardnessRadioButtons();

		ImGui::BeginChildFrame(2, {200, 204}, 4);
		ImGui::Text("     Enter your name");
		 static char str0[128] = "Your name";
		 ImGui::InputText("", str0, IM_ARRAYSIZE(str0));

		ImGui::EndPopup();
	}
	if (ImGui::Button("Start new Campaign", STANDARD_MENU_BUTTON))
	{
		//ImGui::OpenPopup("Select stage");
		CONFIGURATION.setChosenStage(1);
		GamePaused(false);
		StartTheGame(true);
	}
}

void Gui::ShowLoadSavedGamesMenu()
{
	if (ImGui::BeginPopup("Saved Games"))
	{
		if (mButtonsTextures.empty())
		{
			mButtonsTextures.push_back((ImTextureID)RESOURCES.getTexture("unlocked")->getTextureID());
			mButtonsTextures.push_back((ImTextureID)RESOURCES.getTexture("cloud_trans")->getTextureID());
		}

		ImGui::BeginChildFrame(2, {201, 204}, 4);
		ImGui::Text("Choose stage of the campaign");
		if (ImGui::ImageButton(mButtonsTextures.at(CONFIGURATION.getChosenStage() > 0 ? 0 : 1), ImVec2(32, 32), ImVec2(0, 0), ImVec2(32.0f, 32.0f), 2, ImColor(0, 0, 0, 255)))
		{
			CONFIGURATION.setChosenStage(1);
			GamePaused(false);
			StartTheGame(true);
			Game::mReloadStage = true;
		}
		ImGui::SameLine();
		ImGui::Text("\nStage 1");
		if (ImGui::ImageButton(mButtonsTextures.at(CONFIGURATION.getChosenStage() > 1 ? 0 : 1), ImVec2(32, 32), ImVec2(0, 0), ImVec2(32.0f, 32.0f), 2, ImColor(0, 0, 0, 255)))
		{
			CONFIGURATION.setChosenStage(2);
			GamePaused(false);
			StartTheGame(true);
			Game::mReloadStage = true;
		}
		ImGui::SameLine();
		ImGui::Text("\nStage 2");
		if (ImGui::ImageButton(mButtonsTextures.at(CONFIGURATION.getChosenStage() > 2 ? 0 : 1), ImVec2(32, 32), ImVec2(0, 0), ImVec2(32.0f, 32.0f), 2, ImColor(0, 0, 0, 255)))
		{
			CONFIGURATION.setChosenStage(3);
			GamePaused(false);
			StartTheGame(true);
			Game::mReloadStage = true;
		}
		ImGui::SameLine();
		ImGui::Text("\nStage 3");
		if (ImGui::ImageButton(mButtonsTextures.at(CONFIGURATION.getChosenStage() == 0 ? 0 : 1), ImVec2(32, 32), ImVec2(0, 0), ImVec2(32.0f, 32.0f), 2, ImColor(0, 0, 0, 255)))
		{
			CONFIGURATION.setChosenStage(0);
			GamePaused(false);
			StartTheGame(true);
			Game::mReloadStage = true;
		}
		ImGui::SameLine();
		ImGui::Text("\nBonus level");
		ImGui::EndChildFrame();
		ImGui::EndPopup();
	}

	if (ImGui::Button("Continue", STANDARD_MENU_BUTTON))
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
		ImGui::SliderInt("10", &CONFIGURATION.getMusicVolume(), 0, 10, "Music");
		ImGui::Separator();

		ImGui::Text("\nSet sounds volume\n");
		ImGui::SliderInt("9", &CONFIGURATION.getSoundVolume(), 0, 9, "Sounds");
		ImGui::Separator();

		ImGui::Text("\nSet keybinding\n");
		ImGui::RadioButton("Arrows", &CONFIGURATION.getKeyBindVolume(), 0);ImGui::SameLine();
		ImGui::RadioButton("ASWD", &CONFIGURATION.getKeyBindVolume(), 1);ImGui::SameLine();
		ImGui::RadioButton("🎮", &CONFIGURATION.getKeyBindVolume(), 2);ImGui::SameLine();
		ImGui::RadioButton("HJKL", &CONFIGURATION.getKeyBindVolume(), 3);
		ImGui::Separator();

		if (ImGui::Checkbox("FullScreen", &CONFIGURATION.getWindowed()))
		{
			CONFIGURATION.setWindowed(CONFIGURATION.getWindowed());
		}

		ImGui::Text("\nSet screen resolution\n");
         const char* items[] = {"360", "480", "720", "1400"};
		//ImGui::Combo("combo", &CONFIGURATION.getScreenResolution(), "360\0res480\0res720\0res1000\0res1200\0\0");
		ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;

		ImGuiStyle& style = ImGui::GetStyle();
		float w = ImGui::CalcItemWidth();
		float spacing = style.ItemInnerSpacing.x;
		float button_sz = ImGui::GetFrameHeight();
		ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
		if (ImGui::ArrowButton("##r", ImGuiDir_Left))
		{
			if (CONFIGURATION.getScreenResolution() > 0)
				CONFIGURATION.getScreenResolution()--;
		}
		static const char* current_item = items[CONFIGURATION.getScreenResolution()];
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
			if (CONFIGURATION.getScreenResolution() < 4)
				CONFIGURATION.getScreenResolution()++;
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
