#include "Game.hpp"
#include "ResourceManagement/ResourceManager.hpp"

Game::Game() : mIsRunning(true)
{
    mWindow = std::make_unique<GameWindow>(cDefaultScreenWidth, cDefaultScreenHeight, cWindowName);
    mRenderer = std::make_unique<Renderer>();
    mIManager = std::make_unique<InputManager>();
    loadResources();

    mRenderer->initTestData();
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
            break;
        case Action::CameraRotate:
            float x,y;
            mIManager->getMouseOffset(x, y);
            mRenderer->getCamera().processMouseMovement(x, y);
            break;
        default:
            break;
    }
}

void Game::loadResources()
{
    RESOURCES.loadShader("normalModel.vx.glsl", "normalModel.ft.glsl", "modelShader");
    RESOURCES.loadShader("sprite.vx.glsl", "sprite.ft.glsl", "sprite");
    RESOURCES.loadTexture("block.png", "block", false);
    RESOURCES.loadTexture("container.jpg", "container", false);
    RESOURCES.loadTexture("awesomeface.png", "face", true);
}