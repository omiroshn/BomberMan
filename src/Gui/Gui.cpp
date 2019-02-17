
#include <Gui/Gui.h>

Gui::Gui() {

}

Gui::~Gui() {

}

void Gui::ShowMainMenu()
{
	ImGui::SetNextWindowSize(WIN_SIZE, ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowPos({0, 0},0);
	ImGui::SetNextWindowBgAlpha(0.7);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(220,100));
	ImGui::Begin("Main Menu");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 20));
	ImGui::Text("Welcome to BomberMan game!");

	/////////////////////////////////HARDNESS////////////////////////////////////

	//ImGui::Separator();

	ShowHardnessRadioButtons();

	/////////////////////////////////START GAME////////////////////////////////////

	//ImGui::Separator();

	const ImVec2 menu_frame = {200, 120};
	const float spacing = 10;
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, spacing));
	ImGui::BeginChildFrame(1, menu_frame, 4);

	ShowStartNewGameMenu();

	/////////////////////////////////LOAD GAME////////////////////////////////////

	ShowLoadSavedGamesMenu();

	/////////////////////////////////EXIT////////////////////////////////////

	ImGui::Button("EXIT", STANDARD_MENU_BUTTON);

	ImGui::EndChildFrame();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::End();
	ImGui::PopStyleVar();
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

void Gui::ShowStartNewGameMenu()
{
	if (ImGui::BeginPopup("NameFrame"))
	{
		ImGui::BeginChildFrame(2, {200, 40}, 4);
		static char str0[128] = "Your name";
		ImGui::InputText("", str0, IM_ARRAYSIZE(str0));

		ImGui::EndChildFrame();
		ImGui::Button("PLAY", STANDARD_MENU_BUTTON);
		ImGui::EndPopup();
	}
	if (ImGui::Button("Start new GAME", STANDARD_MENU_BUTTON))
	{
		ImGui::OpenPopup("NameFrame");
	}
}

void Gui::ShowLoadSavedGamesMenu()
{
	if (ImGui::BeginPopup("Saved Games"))
	{
		const ImVec2 saved_frame = {200, 300};
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

