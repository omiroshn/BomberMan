#ifndef BOMBERMAN_Game_HPP
#define BOMBERMAN_Game_HPP
#include <memory>
#include "GameWindow.hpp"
#include "GraphicCore/Renderer.hpp"
#include "InputManagement/InputManager.hpp"

namespace
{
    int const cDefaultScreenWidth = 640;
    int const cDefaultScreenHeight = 480;
    std::string cWindowName = "Bomberman";
}


class Game
{
public:
    Game();
    ~Game();
    void start();

private:
    void doAction(Action const&);

private:
    void loadResources();
    std::unique_ptr<GameWindow> mWindow;
    std::unique_ptr<Renderer> mRenderer;
    std::unique_ptr<InputManager> mIManager;

    bool mIsRunning;
};

#endif //BOMBERMAN_Renderer_HPP
