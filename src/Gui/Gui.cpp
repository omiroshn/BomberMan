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
	mWidth = CONFIGURATION.getWidth();
	mHeight = CONFIGURATION.getHeight();
	if (!mBackground)
		mBackground = (ImTextureID)RESOURCES.getTexture("sky")->getTextureID();

	ImGui::SetNextWindowPos({0, 0},0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(mWidth, mHeight));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	ImGui::Begin("Main Menu", NULL, mWindow_flags);
	ImGui::GetWindowDrawList()->AddImage(mBackground, ImVec2(0, 0), ImVec2(mWidth, mHeight));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(mWidth / 2 - 112, mHeight / 2 - 200));
	ImGui::BeginChildFrame(5, ImVec2(mWidth, mHeight), ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2,2));

	if (mCurrentMenu == CurrentMenu::mainMenu)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 20));
		ImGui::Text("       BomberMan game menu");

		/////////////////////////////////START GAME////////////////////////////////////
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 2));
		ImGui::BeginChildFrame(1, {224, 204}, ImGuiWindowFlags_AlwaysAutoResize);

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
			Game::mIsRunning = false;
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
		default:
			break;
	}
}

void Gui::ShowStartNewGameMenu()
{
	if (ImGui::BeginPopup("Select stage"))
	{
		ImGui::BeginChildFrame(4, {200, 204}, 4);
		ImGui::Text("     Enter your name");
		 static char str0[128] = "Your name";
		 ImGui::InputText("", str0, IM_ARRAYSIZE(str0));

		ImGui::EndPopup();
	}
	if (ImGui::Button("Start new Campaign", STANDARD_MENU_BUTTON))
	{
		//ImGui::OpenPopup("Select stage");
		mCurrentMenu = CurrentMenu::inGameMenu;
		CONFIGURATION.setChosenStage(DefaultChosenStage);
		CONFIGURATION.setLives(DefaultLives);
		CONFIGURATION.setBestLevelAchieved(DefaultBestLevelAchieved);
		CONFIGURATION.setScore(DefaultScore);
		GamePaused(false);
		StartTheGame(true);
	}
}

void Gui::ChangeStage(int next_stage)
{
	int ach = CONFIGURATION.getBestLevelAchieved();
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
		if (mButtonsTextures.empty())
		{
			mButtonsTextures.push_back((ImTextureID)RESOURCES.getTexture("unlocked0")->getTextureID());
			mButtonsTextures.push_back((ImTextureID)RESOURCES.getTexture("unlocked1")->getTextureID());
			mButtonsTextures.push_back((ImTextureID)RESOURCES.getTexture("unlocked2")->getTextureID());
			mButtonsTextures.push_back((ImTextureID)RESOURCES.getTexture("unlocked3")->getTextureID());
			mButtonsTextures.push_back((ImTextureID)RESOURCES.getTexture("locked")->getTextureID());
		}
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(28, 4));
		ImGui::BeginChildFrame(2, {234, 204}, 4);
		ImGui::Text("\nStages of the campaign");
		if (ImGui::ImageButton(mButtonsTextures.at(0), ImVec2(32, 32), ImVec2(0, 1), ImVec2(1.0f, 0.0f), 2, ImColor(0, 0, 0, 255)))
		{
			ChangeStage(0);
		}
		ImGui::SameLine();
		ImGui::Text("\n   Stage 1");
		if (ImGui::ImageButton(mButtonsTextures.at(CONFIGURATION.getBestLevelAchieved() > 0 ? 1 : 4), ImVec2(32, 32), ImVec2(0, 1), ImVec2(1.0f, 0.0f), 2, ImColor(0, 0, 0, 255)))
		{
			ChangeStage(1);
		}
		ImGui::SameLine();
		ImGui::Text("\n   Stage 2");
		if (ImGui::ImageButton(mButtonsTextures.at(CONFIGURATION.getBestLevelAchieved() > 1 ? 2 : 4), ImVec2(32, 32), ImVec2(0, 1), ImVec2(1.0f, 0.0f), 2, ImColor(0, 0, 0, 255)))
		{
			ChangeStage(2);
		}
		ImGui::SameLine();
		ImGui::Text("\n   Stage 3");
		if (ImGui::ImageButton(mButtonsTextures.at(CONFIGURATION.getBestLevelAchieved() > 2 ? 3 : 4), ImVec2(32, 32), ImVec2(0, 1), ImVec2(1.0f, 0.0f), 2, ImColor(0, 0, 0, 255)))
		{
			ChangeStage(3);
		}
		ImGui::SameLine();
		ImGui::Text("\n   Bonus level");
		ImGui::EndChildFrame();
		ImGui::PopStyleVar();
		if (ImGui::Button("BACK", {234, 48}))
		{
			mCurrentMenu = CurrentMenu::mainMenu;
			return;
		}
}

void Gui::ShowSettingsMenu()
{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(40, 4));
		const ImVec2 saved_frame = {234, 330};
		ImGui::BeginChildFrame(3, saved_frame, 4);

		ImGui::Text("\n    Set music volume\n");
		ImGui::SliderInt("1O", &CONFIGURATION.getMusicVolume(), 0, 10, "Music");
		ImGui::Separator();

		ImGui::Text("\n   Set sounds volume\n");
		ImGui::SliderInt("10", &CONFIGURATION.getSoundVolume(), 0, 10, "Sounds");
		ImGui::Separator();

		ImGui::Text("\n    Set keybinding\n");
		ImGui::RadioButton("Arrows", &CONFIGURATION.getKeyBindVolume(), 0);ImGui::SameLine();
		ImGui::RadioButton("ASWD", &CONFIGURATION.getKeyBindVolume(), 1);
		ImGui::RadioButton("🎮", &CONFIGURATION.getKeyBindVolume(), 2);ImGui::SameLine();
		ImGui::RadioButton("HJKL", &CONFIGURATION.getKeyBindVolume(), 3);
		ImGui::Separator();

		if (ImGui::Checkbox("     FullScreen", &CONFIGURATION.getWindowed()))
		{
			CONFIGURATION.setWindowed(CONFIGURATION.getWindowed());
		}

		ImGui::Text("\nSet screen resolution\n");
        const char* items[] = {"360", "480", "720", "1400"};
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
	mBackground = (ImTextureID)RESOURCES.getTexture(texture)->getTextureID();
}

void Gui::ShowLoadingScreen(const char* screen)
{
	ImTextureID im = (ImTextureID)RESOURCES.getTexture(screen)->getTextureID();
	ImGui::SetNextWindowPos({0, 0},0);
	mWidth = CONFIGURATION.getWidth();
	mHeight = CONFIGURATION.getHeight();
	ImGui::SetNextWindowSize({mWidth,mHeight});
	ImGui::Begin("Next Stage", NULL, mWindow_flags);
	ImGui::Image(im,{mWidth,mHeight}, {1,1}, {0,0});
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 20));
	ImGui::GetWindowDrawList()->AddText( ImVec2(mWidth / 2 - 50, mHeight / 2), ImColor(1.0f,1.0f,1.0f,1.0f), "Welcome to next stage" );
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