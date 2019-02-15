#include "Game.hpp"
#include "ResourceManagement/ResourceManager.hpp"

Game::Game() : mIsRunning(true)
{
    mWindow = std::make_unique<GameWindow>(cDefaultScreenWidth, cDefaultScreenHeight, cWindowName);
    mRenderer = std::make_unique<Renderer>();
    mIManager = std::make_unique<InputManager>();
    //loadResources();
}

Game::~Game()
{
    RESOURCES.clear();
}

void Game::start()
{
    while (mIsRunning)
    {
        mRenderer->draw();
        mWindow->update();
        doAction(mIManager->processEvents(mWindow->getEvent()));
    }
}

void Game::doAction(Action const& a)
{
    switch (a)
    {
        case Action::Finish:
            mIsRunning = false;
        default:
            break;
    }
}

void Game::loadResources()
{
    RESOURCES.loadShader("model_core.vx.glsl", "model_core.ft.glsl", "modelShader");
    RESOURCES.loadShader("sprite.vx.glsl", "sprite.ft.glsl", "sprite");
    RESOURCES.loadTexture("block.png", "block", false);
}