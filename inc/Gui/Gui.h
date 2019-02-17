#ifndef BOMBERMAN_GUI_H
#define BOMBERMAN_GUI_H

#include <imgui.h>
#include "Gui/imgui_impl_sdl_gl3.h"

class Gui
{
public:
	Gui();
	~Gui();
	void ShowMainMenu();

private:
	bool m_showMenu = true;
	bool m_showScore = false;
};

#endif //BOMBERMAN_GUI_H
