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
bool            Game::mReloadStage = false;
bool            Game::mIsRunning = true;
Uint64          Game::mStageTimer = 200;
Uint64          Game::mStageStartedTimer = 0;

namespace
{
    int const cDefaultScreenWidth = 2048;
    int const cDefaultScreenHeight = 1024;
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
        CONFIGURATION.setHeight(height);
        CONFIGURATION.setWidth(width);
        if (!mWindow.get()->IsGameRunning())
        {
            mWindow.get()->ShowStartingMenu();
        }
        else
        {
            if (mapLoader.MapIsLoaded() && !mReloadStage)
            {
                MovingEntity::debugMovement();
                updateHeroInput();
                Tickable::tickTickables(mDeltaTime);
                resolveCollisions();
                mRenderer->draw(mMap);
                static int index = 0;
                ImGui::RadioButton("NO VSync", &index, 0);
                ImGui::RadioButton("60", &index, 1);
                ImGui::RadioButton("30", &index, 2);
                if (index)
                {
                    const float TargetDelta = 0.0167f * index;
					unsigned ms = TargetDelta - mDeltaTime;
                    if (mDeltaTime < TargetDelta)
                        SDL_Delay(ms * 1000);
                }
                mStageTimer = 200 - (getCurrentTime() - mStageStartedTimer);
                mWindow->ShowInGameMenu();
            }
            else
            {
                if (CONFIGURATION.getLives() == 0)
                    CONFIGURATION.setChosenStage(1);
                mMap.cleanMapForRendering();
                mCollisionInfo.Squares.clear();
                std::tie(mMap, mCollisionInfo) = mapLoader.GetMap(CONFIGURATION.getChosenStage());
                mStageStartedTimer = getCurrentTime();
                mReloadStage = 0;
            }
        }
        doAction(mIManager->processEvents(mWindow->getEvent()));
		mRenderer->getParticleManager()->update();
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

CollisionInfo& Game::getCollisionInfo()
{
	return mCollisionInfo;
}

bool circle_box_collision(glm::vec2 position, float radius, glm::vec2 min, glm::vec2 max, glm::vec2 *resolutionOffset = NULL)
{
    glm::vec2 PossibleCollisionPoint = glm::clamp(position, min, max);
    glm::vec2 CollisionDirection = position - PossibleCollisionPoint;
    float distance = glm::length(CollisionDirection);
    bool overlaps = distance < radius;
    if (!overlaps)
        return false;

    bool inside = position.x > min.x && position.y > min.y
               && position.x < max.x && position.y < max.y;

    if (resolutionOffset)
    {
        glm::vec2 CollisionNormal;
        if (inside)
            CollisionNormal = -CollisionDirection;
        else
            CollisionNormal = CollisionDirection / distance;
        float coefficient = (radius - distance) / radius;
        *resolutionOffset = (CollisionNormal) * (coefficient);
    }

    return true;
}

void Game::resolveCollisions()
{
	glm::vec2 offsets[] = {
		{-1, -1},
		{1, -1},
		{-1, 1},
		{-1, -1},
		{1 , 0},
		{-1, 0},
		{0, 1},
		{0, -1}
    };

	auto& Hero = mMap.GetHero();
    glm::vec2 CollisionOffset(0);
	const glm::vec2 Position = Hero.getPosition();
	static float radius = 0.24;
    ImGui::SliderFloat("Collision radius", &radius, 0.05, 1);
    for (int i = 0; i < ARRAY_COUNT(offsets); i++)
    {
        glm::vec2 ProbePoint = Position + offsets[i];
        bool inObstacle = mCollisionInfo[ProbePoint] != SquareType::EmptySquare;
        if (!inObstacle)
            continue;
        glm::vec2 ResolutionOffset;
        if (circle_box_collision(Position, radius, glm::floor(ProbePoint), glm::ceil(ProbePoint), &ResolutionOffset))
            CollisionOffset += ResolutionOffset;
    }

    // Hero.move(CollisionOffset);
    static float CollisionResolveMultiplier = 350.f;
    ImGui::SliderFloat("CollisionResolveMultiplier", &CollisionResolveMultiplier, 100, 1000);
    Hero.AddAcceleration(CollisionOffset * CollisionResolveMultiplier);
	mRenderer->getCamera().followEntity(mMap.GetHero(), 10.f);
}

short x_move, y_move;

void Game::doAction(Action const& a)
{
    auto& Hero = mMap.GetHero();
    const float offset  = mDeltaTime * sInputAcceleration;
    
    auto *joystick = mWindow->getJoystick();
    x_move = SDL_JoystickGetAxis(joystick, 0);
    y_move = SDL_JoystickGetAxis(joystick, 1);
    
    glm::vec2 normalizedJoystick(
        x_move / (float)MAX_JOYSTICK_VALUE,
        y_move / (float)MAX_JOYSTICK_VALUE
    );
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
        case Action::Backward:
            //mRenderer->getCamera().movaCamera(CameraDirection::BACKWARD, mDeltaTime);
        case Action::Right:
            //mRenderer->getCamera().movaCamera(CameraDirection::RIGHT, mDeltaTime);
        case Action::Left:
            //mRenderer->getCamera().movaCamera(CameraDirection::LEFT, mDeltaTime);
        case Action::UpLeft:
        case Action::UpRight:
        case Action::DownLeft:
        case Action::DownRight:
            Hero.AddAcceleration(normalizedJoystick * offset);
            break;
        case Action::Up:
        case Action::Down:
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
        RESOURCES.loadSkybox("blue");
        RESOURCES.loadSkybox("lightblue");

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

void Game::explosion(glm::ivec2 position, uint32_t span)
{
	static int which;
	static std::string brickPool[] {
		"BrickBlock", 
		"BrickBlock2", 
	};
	static std::string bombPool[] {
		"pointSphereBomb",
		"pointSphereBomb2",
	};
	

    glm::vec2 directions[] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    glm::ivec2 minMax[] = {
        position,
        position + glm::ivec2(1),
        position,
        position + glm::ivec2(1)
    };

    glm::vec2 centerPosition = glm::vec2(position) + glm::vec2(0.5f);

    std::vector<glm::mat4> fireTransforms;
    std::vector<glm::mat4> brickTransforms;
    fireTransforms.push_back(glm::translate(glm::mat4(1), glm::vec3(centerPosition.x, 0, centerPosition.y)));

    for (int i = 0; i < ARRAY_COUNT(directions); i++)
    for (int j = 1; j <= span; ++j)
    {
        glm::vec2 testPosition = centerPosition + (float)j * directions[i];
        auto& type = mCollisionInfo[testPosition];
        if (type == SquareType::Wall)
            break;

        fireTransforms.push_back(glm::translate(glm::mat4(1), glm::vec3(testPosition.x, 0, testPosition. y)));
        minMax[i] += directions[i];

        if (type == SquareType::Brick)
        {
            brickTransforms.push_back(glm::translate(glm::mat4(1), glm::vec3(testPosition.x, 0, testPosition. y)));
            type = SquareType::EmptySquare;
            break;
        }
    }

    auto hMin = minMax[0];
    auto hMax = minMax[1];
    auto vMin = minMax[2];
    auto vMax = minMax[3];

    auto &rawMap = mMap.GetRawMap();
    std::remove_if(rawMap.begin(), rawMap.end(), [this, hMin, hMax, vMin, vMax](SquareInstance *instance) {
        return instance->GetType() == SquareType::Brick
        && (circle_box_collision(instance->getPosition() + glm::vec2(0.5), 0.001, hMin, hMax)
        || circle_box_collision(instance->getPosition() + glm::vec2(0.5), 0.001, vMin, vMax));
    });

    auto &enemies = mMap.GetEnemies();
    std::remove_if(enemies.begin(), enemies.end(), [this, hMin, hMax, vMin, vMax](MovingEntity *entity) {
        return (circle_box_collision(entity->getPosition() + glm::vec2(0.5), 0.1, hMin, hMax)
        || circle_box_collision(entity->getPosition() + glm::vec2(0.5), 0.1, vMin, vMax));
    });

    mRenderer->getParticleManager()->startDrawPS(brickPool[which], brickTransforms);
    mRenderer->getParticleManager()->startDrawPS(bombPool[which], fireTransforms);
	which = !which;
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
    if (ImGui::IsKeyPressed(SDL_SCANCODE_0))
        explosion(Hero.getPosition(), 10);
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
