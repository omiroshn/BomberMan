#include "Game.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "ResourceManagement/Model.hpp"
#include "Core.hpp"
#include "Entity/MovingEntity.h"
#include <tuple>
#include "AI/AIController.h"
#include "LogicCore/Timer.h"
#include "LogicCore/TimerManager.h"

Uint64			Game::mTimeNow;
Uint64			Game::mTimeLast;
float			Game::mDeltaTime;
CollisionInfo	Game::mCollisionInfo;

bool  Game::mIsRunning = true;

namespace
{
    std::string const cWindowName = "Bomberman";
}

Game::Game()
{
	mTimeNow = SDL_GetPerformanceCounter();

	loadStateFromFile();
    mWindow = std::make_shared<GameWindow>(CONFIGURATION.getWidth(), CONFIGURATION.getHeight(), cWindowName);
	mWindow->setFullScreen(CONFIGURATION.getWindowed());
	CONFIGURATION.setObservableWindow(mWindow);

	mRenderer = std::make_unique<Renderer>();
    mIManager = std::make_unique<InputManager>();
    // timerManager = TimerManager::Instance();
    loadResources();

}

Game::~Game()
{
    RESOURCES.clear();
}

void Game::start()
{
    MapLoader mapLoader;
    int width, height;

    // auto lambda1 = []() { std::cout << ":)" << std::endl; };
    // timerManager->AddTimer(1, false, lambda1);
    while (mIsRunning)
    {
        // timerManager->Update();
        mWindow->tickGui();
        mWindow->getSize(width, height);
        calcDeltaTime();
        mRenderer->updateSize(width, height);
        if (!mWindow.get()->IsGameRunning())
        {
            mWindow.get()->ShowStartingMenu();
        }
        else
        {
            if (mapLoader.MapIsLoaded())
            {
                MovingEntity::debugMovement();
                updateHeroInput();
                bm::Tickable::tickTickables(mDeltaTime);
                resolveCollisions();
                mRenderer->draw(mMap);
                static int index = 0;
                ImGui::RadioButton("NO VSync", &index, 0);
                ImGui::RadioButton("60", &index, 1);
                ImGui::RadioButton("30", &index, 2);
                if (index)
                {
                    const float TargetDelta = 0.0167f * index;
                    if (mDeltaTime < TargetDelta)
                        SDL_Delay(TargetDelta - mDeltaTime * 1000);
                }
                mWindow->ShowInGameMenu();
                mapLoader.UpdateMap();
            }
            else
            {
                std::tie(mMap, mCollisionInfo) = mapLoader.GetMap(CONFIGURATION.getChosenStage());
                mMap.ParseMapBySquareInstances();
                mMap.GetEnemies().reserve(10);
                for (int i = 1; i <= 4; i++)
                {
                    auto& Balloon = mMap.GetEnemies().emplace_back(glm::vec2(7 + i, 1));
                    AIController::addBalloon(Balloon);
                }
            }

        }
        doAction(mIManager->processEvents(mWindow->getEvent()));
        mWindow->update();
    }
}

void Game::pause()
{
    mIsPaused = !mIsPaused;
    mWindow.get()->PauseGame(mIsPaused);
}

float Game::getCurrentTime()
{
	return mTimeNow / static_cast<float>(SDL_GetPerformanceFrequency());
}

const CollisionInfo& Game::getCollisionInfo()
{
	return mCollisionInfo;
}

void Game::resolveCollisions()
{
	auto& Hero = mMap.GetHero();
	const glm::vec2 Position = Hero.getPosition() + 0.5f;
	bool inObstacle = mCollisionInfo[Position] != SquareType::EmptySquare;
	if (inObstacle)
	{
		glm::vec2 CorrectedPosition = glm::round(Position);
		CorrectedPosition -= Position;
		if (fabs(CorrectedPosition.x) < fabs(CorrectedPosition.y))
			Hero.move({CorrectedPosition.x, 0});
		else
			Hero.move({0, CorrectedPosition.y});
	}
    mRenderer->getCamera().followEntity(Hero, 10.f);
}

void Game::doAction(Action const& a)
{
	std::vector<glm::mat4> transforms;
	std::vector<glm::mat4> transforms1;
    switch (a)
    {
        case Action::Finish:
            mIsRunning = false;
			saveCurrentState();
            break;
        case Action::Pause:
            pause();
            break;
        case Action::CameraRotate:
            float x,y;
            mIManager->getMouseOffset(x, y);
            mRenderer->getCamera().processMouseMovement(x, y);
            break;
        case Action::Forward:
            //mRenderer->getCamera().movaCamera(CameraDirection::FORWARD, mDeltaTime);
            break;
        case Action::Backward:
            //mRenderer->getCamera().movaCamera(CameraDirection::BACKWARD, mDeltaTime);
            break;
        case Action::Right:
            //mRenderer->getCamera().movaCamera(CameraDirection::RIGHT, mDeltaTime);
            break;
        case Action::Left:
            //mRenderer->getCamera().movaCamera(CameraDirection::LEFT, mDeltaTime);
            break;
        case Action::Up:
			transforms.clear();
			for (int i = 1; i < 10; ++i){
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(2.0f * i ,1.5f,1.5f  + 2.0f * i));
				transforms.push_back(model);
			}
			try {
			//mRenderer->getParticleManager()->startDrawPS("pointSphereBomb", transforms);
				mRenderer->getParticleManager()->startDrawPS("Cloud", transforms);
				//mRenderer->getParticleManager()->startDrawPS("quadSphereBombTMap", transforms);
			} catch (CustomException &ex) {
				std::cout << ex.what() << std::endl;
				exit(42);
			}
//            mRenderer->getCamera().movaCamera(CameraDirection::UPWARD, mDeltaTime);
            break;
        case Action::Down:
			transforms.clear();
			transforms1.clear();
			for (int i = 0; i < 10; ++i){
				glm::mat4 model = glm::mat4(1.0f);
				//model =  glm::translate(model, glm::vec3(-2.5f, 0.0f, -2.5f));
				model = glm::translate(model, glm::vec3(3.0f ,0.0f,2.0f* (i + 1)));
				transforms.push_back(model);
				model = glm::translate(model, glm::vec3(0.08f ,-0.125f,-0.25));
				transforms1.push_back(model);
			}
			try {
				mRenderer->getParticleManager()->startDrawPS("BrickBlock", transforms1);
				mRenderer->getParticleManager()->startDrawPS("quadSphereBomb", transforms);
			} catch (CustomException &ex) {
				std::cout << ex.what() << std::endl;
				exit(42);
			}
			//loadStateFromFile();
            //mRenderer->getCamera().movaCamera(CameraDirection::DOWNWARD, mDeltaTime);
            break;
        default:
            break;
    }
}

void Game::calcDeltaTime()
{
    mTimeLast = mTimeNow;
    mTimeNow = SDL_GetPerformanceCounter();

	mDeltaTime = (mTimeNow - mTimeLast) / static_cast<float>(SDL_GetPerformanceFrequency());
	if (mDeltaTime > 1.f)
		mDeltaTime = 0.016f;
	ImGui::Text("Current time: %f", getCurrentTime());
	ImGui::Text("Delta time: %f", mDeltaTime);
}

void Game::loadResources()
{
    try
    {
        RESOURCES.loadShader("sprite.vx.glsl", "sprite.ft.glsl", "sprite");
        RESOURCES.loadShader("modelShader.vx.glsl", "modelShader.ft.glsl", "modelShader");
        RESOURCES.loadShader("skybox.vx.glsl", "skybox.ft.glsl", "skybox");
		RESOURCES.loadShader("sprite_p.vx.glsl", "sprite_p.ft.glsl", "sprite_p");
		RESOURCES.loadShader("sprite_quad.vx.glsl", "sprite_quad.ft.glsl", "sprite_quad");
		RESOURCES.loadShader("sprite_quad_brick.vx.glsl", "sprite_quad.ft.glsl", "sprite_quad_brick");
		RESOURCES.loadShader("sprite_quad_cloud.vx.glsl", "sprite_quad_cloud.ft.glsl", "sprite_quad_cloud");
        RESOURCES.loadTexture("block.png", "block");
        RESOURCES.loadTexture("unlocked.png", "unlocked");
        RESOURCES.loadTexture("container.jpg", "container");
        RESOURCES.loadTexture("awesomeface.png", "face");
		RESOURCES.loadTexture("flame-fire.png", "flame-fire");
		RESOURCES.loadTexture("cloud_trans.jpg", "cloud_trans");
		RESOURCES.loadTexture("explode.png", "explosion_tmap_2");
        RESOURCES.loadSkybox("defaultSkybox");

        auto a = RESOURCES.loadModel("y_bot/y_bot.fbx", "bot");
        {
            glm::mat4 basicSuite = glm::mat4(1.0f);
            auto  scale_factorY = (a->getAABB().getMax().y - a->getAABB().getMin().y);
            basicSuite = glm::scale(basicSuite, glm::vec3(1.0f / scale_factorY, 1.0f / scale_factorY, 1.0f / scale_factorY));
			basicSuite = glm::translate(basicSuite, -a->getAABB().getCenter());
            a->transform(basicSuite);
        }
        auto b = RESOURCES.loadModel("brick/brick.obj", "brick");
        {
            glm::mat4 basicCube = glm::mat4(1.0f);
            auto scale_factorY = 2.0f;
            basicCube = glm::scale(basicCube,  glm::vec3(1.f / scale_factorY));
			basicCube = glm::translate(basicCube, -b->getAABB().getCenter());
            basicCube = glm::rotate(basicCube, glm::radians(-90.0f), glm::vec3(0,0,1));
            b->transform(basicCube);
        }
        auto c = RESOURCES.loadModel("ground/ground.obj", "ground");
        {
            glm::mat4 basicCube = glm::mat4(1.0f);
            auto scale_factorY = 2.0f;
            basicCube = glm::scale(basicCube,  glm::vec3(1.f / scale_factorY));
            basicCube = glm::translate(basicCube, -c->getAABB().getCenter());
            c->transform(basicCube);
        }
        auto d = RESOURCES.loadModel("wall/wall.obj", "wall");
        {

            glm::mat4 basicCube = glm::mat4(1.0f);
            auto scale_factorY = 2.0f;
            basicCube = glm::scale(basicCube,  glm::vec3(1.f / scale_factorY));
			basicCube = glm::translate(basicCube, -c->getAABB().getCenter());
            d->transform(basicCube);
        }
        {
            auto Model = RESOURCES.loadModel("balloon/balloon.obj", "balloon");
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            auto scale_factorY = 200;
            modelMatrix = glm::scale(modelMatrix,  glm::vec3(1.f / scale_factorY));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0,-280,0));
			modelMatrix = glm::translate(modelMatrix, -c->getAABB().getCenter());
            Model->transform(modelMatrix);
        }
		mRenderer->getParticleManager()->init();
    }
    catch (CustomException &e)
    {
        std::cout << e.what() << std::endl;
        exit(42);
    }
}

void Game::updateHeroInput()
{
	auto& Hero = mMap.GetHero();
	ImGui::SliderFloat("Input Hero acceleration", &sInputAcceleration, 0, 10000);
	const float offset  = mDeltaTime * sInputAcceleration;
	if (ImGui::IsKeyDown(SDL_SCANCODE_LEFT))
		Hero.AddAcceleration(glm::vec2(-offset, 0));
	if (ImGui::IsKeyDown(SDL_SCANCODE_RIGHT))
		Hero.AddAcceleration(glm::vec2(offset, 0));
	if (ImGui::IsKeyDown(SDL_SCANCODE_UP))
		Hero.AddAcceleration(glm::vec2(0, -offset));
	if (ImGui::IsKeyDown(SDL_SCANCODE_DOWN))
		Hero.AddAcceleration(glm::vec2(0, offset));

    if (ImGui::IsKeyDown(SDL_SCANCODE_A))
        mRenderer->getCamera().movaCamera(CameraDirection::LEFT, mDeltaTime * 100);
	if (ImGui::IsKeyDown(SDL_SCANCODE_S))
        mRenderer->getCamera().movaCamera(CameraDirection::BACKWARD, mDeltaTime * 100);
	if (ImGui::IsKeyDown(SDL_SCANCODE_D))
        mRenderer->getCamera().movaCamera(CameraDirection::RIGHT, mDeltaTime * 100);
	if (ImGui::IsKeyDown(SDL_SCANCODE_W))
        mRenderer->getCamera().movaCamera(CameraDirection::FORWARD, mDeltaTime * 100);
}

void 		Game::saveCurrentState(std::string fileName)
{
	int w, h;
	mWindow->getSize(w,h);
	CONFIGURATION.setSize(w,h);
	CONFIGURATION.serialise(fileName);
}

void 		Game::loadStateFromFile(std::string fileName)
{
	CONFIGURATION.deserialise(fileName);
	applyWindowChange();
}

void 		Game::applyWindowChange()
{
	if (mWindow)
	{
		mWindow->setSize(CONFIGURATION.getWidth(), CONFIGURATION.getHeight());
		mWindow->setFullScreen(CONFIGURATION.getWindowed());
	}
}

float Game::sInputAcceleration = 6000;
