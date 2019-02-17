#include "Game.hpp"
#include "ResourceManagement/ResourceManager.hpp"

Game::Game() : mTimeNow(SDL_GetPerformanceCounter()), mIsRunning(true)
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
        mDeltaTime = calcDeltaTime();
        mWindow->tickGui();
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
        case Action::Forward:
            mRenderer->getCamera().movaCamera(CameraDirection::FORWARD, mDeltaTime);
            break;
        case Action::Backward:
            mRenderer->getCamera().movaCamera(CameraDirection::BACKWARD, mDeltaTime);
            break;
        case Action::Right:
            mRenderer->getCamera().movaCamera(CameraDirection::RIGHT, mDeltaTime);
            break;
        case Action::Left:
            mRenderer->getCamera().movaCamera(CameraDirection::LEFT, mDeltaTime);
            break;
        case Action::Up:
            mRenderer->getCamera().movaCamera(CameraDirection::UPWARD, mDeltaTime);
            break;
        case Action::Down:
            mRenderer->getCamera().movaCamera(CameraDirection::DOWNWARD, mDeltaTime);
            break;
        default:
            break;
    }
}

float Game::calcDeltaTime()
{
    mTimeLast = mTimeNow;
    mTimeNow = SDL_GetPerformanceCounter();

    return ((static_cast<float>(mTimeNow - mTimeLast) * 1000.0f / static_cast<float>(SDL_GetPerformanceFrequency())));
}

void Game::loadResources()
{
    try
    {
        RESOURCES.loadShader("normalModel.vx.glsl", "normalModel.ft.glsl", "normalModel");
        RESOURCES.loadShader("sprite.vx.glsl", "sprite.ft.glsl", "sprite");
        RESOURCES.loadShader("modelShader.vx.glsl", "modelShader.ft.glsl", "modelShader");
        RESOURCES.loadTexture("block.png", "block");
        RESOURCES.loadTexture("container.jpg", "container");
        RESOURCES.loadTexture("awesomeface.png", "face");
        RESOURCES.loadModel("nanosuit/nanosuit.obj", "nanosuit");
        RESOURCES.loadModel("brick/brick.obj", "brick");
    }
    catch (CustomException &what)
    {
        std::cout << what.text << std::endl;
        exit(42);
    }
}