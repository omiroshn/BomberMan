#include "Game.hpp"
#include "ResourceManagement/ResourceManager.hpp"

Game::Game() : mTimeNow(SDL_GetPerformanceCounter()), mIsRunning(true)
{
    mWindow = std::make_unique<GameWindow>(cDefaultScreenWidth, cDefaultScreenHeight, cWindowName);
    mRenderer = std::make_unique<Renderer>();
    mIManager = std::make_unique<InputManager>();
    loadResources();
}

Game::~Game()
{
    RESOURCES.clear();
}

void Game::start()
{
    MapLoader mapLoader;
    while (mIsRunning)
    {
        mDeltaTime = calcDeltaTime();
        mWindow->tickGui();
        if (mapLoader.MapIsLoaded())
        {
            mRenderer->draw(mMap);
            mWindow->update();
            doAction(mIManager->processEvents(mWindow->getEvent()));
            mapLoader.UpdateMap();
        }
        else
        {
            mMap = std::make_shared<MapForRendering>(mapLoader.GetMap(-1));
            mMap.get()->ParseMapBySquareInstances();
            mWindow->update();
        }
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

        auto a = RESOURCES.loadModel("nanosuit/nanosuit.obj", "nanosuit");
        {
            glm::mat4 basicSuite = glm::mat4(1.0f);
            auto  scale_factorY = (a->getAABB().getMax().y - a->getAABB().getMin().y);
            basicSuite = glm::scale(basicSuite, glm::vec3(1.0f / scale_factorY, 1.0f / scale_factorY, 1.0f / scale_factorY));
            basicSuite = glm::translate(basicSuite, glm::vec3(-a->getAABB().getCenter().x, -a->getAABB().getCenter().y, -a->getAABB().getCenter().z));
            a->transform(basicSuite);
//            std::cout << a->getAABB().getMin().x << " -- " << a->getAABB().getMin().y << " -- " << a->getAABB().getMin().z << std::endl;
//            std::cout << a->getAABB().getMax().x << " -- " << a->getAABB().getMax().y << " -- " << a->getAABB().getMax().z << std::endl;
//            std::cout << a->getAABB().getCenter().x << " -- " << a->getAABB().getCenter().y << " -- " << a->getAABB().getCenter().z << std::endl << std::endl;
        }



        auto b = RESOURCES.loadModel("brick/brick.obj", "brick");
        {

            glm::mat4 basicCube = glm::mat4(1.0f);
            auto scale_factorY = 2.0f;
            basicCube = glm::scale(basicCube, glm::vec3(1.0f / scale_factorY, 1.0f / scale_factorY, 1.0f / scale_factorY));
            basicCube = glm::translate(basicCube, glm::vec3(-b->getAABB().getCenter().x, -b->getAABB().getCenter().y, -b->getAABB().getCenter().z));
            basicCube = glm::rotate(basicCube, glm::radians(-90.0f), glm::vec3(0,0,1));
            b->transform(basicCube);
//            std::cout << b->getAABB().getMin().x << " -- " << b->getAABB().getMin().y << " -- " << b->getAABB().getMin().z << std::endl;
//            std::cout << b->getAABB().getMax().x << " -- " << b->getAABB().getMax().y << " -- " << b->getAABB().getMax().z << std::endl;
//            std::cout << b->getAABB().getCenter().x << " -- " << b->getAABB().getCenter().y << " -- " << b->getAABB().getCenter().z << std::endl;
        }
        auto c = RESOURCES.loadModel("ground/ground.obj", "ground");
        {

            glm::mat4 basicCube = glm::mat4(1.0f);
            auto scale_factorY = 2.0f;
            basicCube = glm::scale(basicCube,
                                   glm::vec3(1.0f / scale_factorY, 1.0f / scale_factorY, 1.0f / scale_factorY));
            basicCube = glm::translate(basicCube, glm::vec3(-c->getAABB().getCenter().x, -c->getAABB().getCenter().y,
                                                            -c->getAABB().getCenter().z));
            c->transform(basicCube);
        }
        auto d = RESOURCES.loadModel("wall/wall.obj", "wall");
        {

            glm::mat4 basicCube = glm::mat4(1.0f);
            auto scale_factorY = 2.0f;
            basicCube = glm::scale(basicCube,
                                   glm::vec3(1.0f / scale_factorY, 1.0f / scale_factorY, 1.0f / scale_factorY));
            basicCube = glm::translate(basicCube, glm::vec3(-c->getAABB().getCenter().x, -c->getAABB().getCenter().y,
                                                            -c->getAABB().getCenter().z));
            d->transform(basicCube);
        }
    }
    catch (CustomException &what)
    {
        std::cout << what.text << std::endl;
        exit(42);
    }
}