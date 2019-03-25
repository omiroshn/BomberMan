#include "Game.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "ResourceManagement/Model.hpp"
#include "Core.hpp"
#include "LogicCore/MovingEntity.h"
#include <tuple>
#include "AI/Brains.h"

Game::Game() : mIsRunning(true)
{
	mTimeNow = SDL_GetPerformanceCounter();
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
    int width, height;

	bm::Brains b;

    while (mIsRunning)
    {
        mWindow->getSize(width, height);
        mWindow->tickGui();
        calcDeltaTime();
        mRenderer->updateSize(width, height);
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
				const float TargetDelta = 0.0167 * index;
				if (mDeltaTime < TargetDelta)
					SDL_Delay((TargetDelta - mDeltaTime) * 1000.f);
			}
            mWindow->update();
            doAction(mIManager->processEvents(mWindow->getEvent()));
            mapLoader.UpdateMap();
        }
        else
        {
            std::tie(mMap, mCollisionInfo) = mapLoader.GetMap(-1);
            mMap.ParseMapBySquareInstances();
            mWindow->update();
        }
    }
}

float Game::getCurrentTime()
{
	return mTimeNow / static_cast<float>(SDL_GetPerformanceFrequency());
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
            break;
        case Action::CameraRotate:
            //float x,y;
            //mIManager->getMouseOffset(x, y);
            //mRenderer->getCamera().processMouseMovement(x, y);
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
			//	mRenderer->getParticleManager()->startDrawPS("BrickBlock", transforms);

			} catch (CustomException &ex) {
				std::cout << ex.what() << std::endl;
				exit(42);
			}
            //mRenderer->getCamera().movaCamera(CameraDirection::UPWARD, mDeltaTime);
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
		RESOURCES.loadShader("sprite_quad_cloud.vx.glsl", "sprite_quad_cloud.ft.glsl", "sprite_quad_cloud");
        RESOURCES.loadTexture("block.png", "block");
        RESOURCES.loadTexture("container.jpg", "container");
        RESOURCES.loadTexture("awesomeface.png", "face");
		RESOURCES.loadTexture("flame-fire.png", "flame-fire");
		RESOURCES.loadTexture("cloud_trans.jpg", "cloud_trans");
		RESOURCES.loadTexture("explode.png", "explosion_tmap_2");
        RESOURCES.loadSkybox("defaultSkybox");

        auto a = RESOURCES.loadModel("nanosuit/nanosuit.obj", "nanosuit");
        {
            glm::mat4 basicSuite = glm::mat4(1.0f);
            auto  scale_factorY = (a->getAABB().getMax().y - a->getAABB().getMin().y);
            basicSuite = glm::scale(basicSuite, glm::vec3(1.0f / scale_factorY, 1.0f / scale_factorY, 1.0f / scale_factorY));
			basicSuite = glm::translate(basicSuite, -a->getAABB().getCenter());
            a->transform(basicSuite);
//            std::cout << a->getAABB().getMin().x << " -- " << a->getAABB().getMin().y << " -- " << a->getAABB().getMin().z << std::endl;
//			 std::cout << a->getAABB().getMax().x << " -- " << a->getAABB().getMax().y << " -- " << a->getAABB().getMax().z << std::endl;
//            std::cout << a->getAABB().getCenter().x << " -- " << a->getAABB().getCenter().y << " -- " << a->getAABB().getCenter().z << std::endl << std::endl;
        }
        auto b = RESOURCES.loadModel("brick/brick.obj", "brick");
        {

            glm::mat4 basicCube = glm::mat4(1.0f);
            auto scale_factorY = 2.0f;
            basicCube = glm::scale(basicCube,  glm::vec3(1.f / scale_factorY));
			basicCube = glm::translate(basicCube, -b->getAABB().getCenter());
            basicCube = glm::rotate(basicCube, glm::radians(-90.0f), glm::vec3(0,0,1));
            b->transform(basicCube);
//            std::cout << b->getAABB().getMin().x << " -- " << b->getAABB().getMin().y << " -- " << b->getAABB().getMin().z << std::endl;
//           std::cout << b->getAABB().getMax().x << " -- " << b->getAABB().getMax().y << " -- " << b->getAABB().getMax().z << std::endl;
//            std::cout << b->getAABB().getCenter().x << " -- " << b->getAABB().getCenter().y << " -- " << b->getAABB().getCenter().z << std::endl;
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
		mRenderer->getParticleManager()->init();
    }
    catch (CustomException &what)
    {
        std::cout << what.text << std::endl;
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
}

float Game::sInputAcceleration = 6000;
