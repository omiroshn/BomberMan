#pragma once

#include <imgui.h>
#include <Gui/Gui.h>
#include "StateMachine/StateMachine.h"

#include <iostream>
class Gui;

struct NewGameMenu;
struct GameMenu;
struct Settings;
struct Exit;

struct MainMenu : public State {
    bool StartPressed;
    bool SettingsPressed;
    bool ExitPressed;

	void onTick(Gui& owner, float DeltaTime) {
		StartPressed = ImGui::Button("Start new game");
		SettingsPressed = ImGui::Button("Settings");
		ExitPressed = ImGui::Button("Exit");

	};

	using State::transition;
	bool transition(const NewGameMenu&)
	{
        std::cout << "Strrtttttt" << std::endl;
        return StartPressed;
	}
	bool transition(const Settings&)
	{
        return SettingsPressed;
	}
	bool transition(const Exit&)
	{
        return ExitPressed;
	}
};

struct NewGameMenu : public State {
};

struct GameMenu : public State {
};

struct Settings : public State {
	void onTick(Gui& owner, float DeltaTime) {
		//ImGui::SliderFloat("Music volume", &owner->musicVolume, 0, 1);
	};

	using State::transition;
	bool transition(const MainMenu&)
	{
		return ImGui::Button("Back to main menu");
	}
};

struct Exit : public State {
};


typedef SM<Gui, MainMenu, NewGameMenu, Settings, GameMenu> GuiState;