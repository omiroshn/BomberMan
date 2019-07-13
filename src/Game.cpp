#include "Game.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "ResourceManagement/Model.hpp"
#include "Core.hpp"
#include "Entity/MovingEntity.h"
#include <tuple>
#include "AI/AIController.h"
#include "LogicCore/Timer.h"
#include "LogicCore/TimerManager.h"
#include "Configure.hpp"

Uint64			Game::mTimeNow;
Uint64			Game::mTimeLast;
float			Game::mDeltaTime;
CollisionInfo	Game::mCollisionInfo;
bool            Game::mReloadStage = true;
bool            Game::mIsRunning = true;
float           Game::mStageTimer = 200;
float           Game::mStageStartedTimer = 0;
float           Game::sInputAcceleration = 6000;
Game			*Game::sInstance = nullptr;

namespace
{
    int const cDefaultScreenWidth = 2048;
    int const cDefaultScreenHeight = 1024;
    std::string const cWindowName = "Bomberman";
}

Game::Game()
	: mHero(std::make_unique<Hero>(Hero::SaveInfo()))
{
	mTimeNow = SDL_GetPerformanceCounter();
    
	loadStateFromFile();
    mWindow = std::make_shared<GameWindow>(CONFIGURATION.getWidth(), CONFIGURATION.getHeight(), cWindowName);
	mWindow->setFullScreen(CONFIGURATION.getWindowed());
	CONFIGURATION.setObservableWindow(mWindow);

	mRenderer = std::make_unique<Renderer>();
    mIManager = std::make_unique<InputManager>();
    mKeyHandler = std::make_unique<KeyboardHandler>();
    loadResources();

    sInstance = this;
}

Game::~Game()
{
    RESOURCES.clear();
}

void Game::start()
{
    MapLoader mapLoader;
    int width, height;
    mStageStartedTimer = getCurrentTime();
    while (mIsRunning)
    {
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
                tickAI(mDeltaTime);
                MovingEntity::debugMovement();
                resolveCollisions();
                Tickable::tickTickables(mDeltaTime);
                mRenderer->draw(*this);
                static int index = 0;
                ImGui::RadioButton("NO VSync", &index, 0);
                ImGui::RadioButton("60", &index, 1);
                ImGui::RadioButton("30", &index, 2);
                if (index)
                {
                    const float TargetDelta = 0.0167f * (float)index;
					float ms = TargetDelta - mDeltaTime;
                    if (mDeltaTime < TargetDelta)
                        SDL_Delay(static_cast<Uint32>(ms * 1000));
                }
                mStageTimer = 200 - (getCurrentTime() - mStageStartedTimer);
                mWindow->ShowInGameMenu();
            }
            else
            {
                if (CONFIGURATION.getLives() == 0)
                    CONFIGURATION.setChosenStage(1);
                if (mReloadStage && mStageTimer > 1)
                {
                    mWindow->ShowBetweenStageScreen();
                    mStageTimer = 4 - (getCurrentTime() - mStageStartedTimer);
                }
                else if (mStageTimer < 2)
                {
                    mapLoader.cleanMapForRendering();
                    mCollisionInfo.Squares.clear();
                    mCollisionInfo = mapLoader.GetMap(CONFIGURATION.getChosenStage());
					// we should store it somewhere! TODO: LIUDOK
					Hero::SaveInfo info;
					mHero = std::make_unique<Hero>(info);
                    mStageStartedTimer = getCurrentTime();
                    mReloadStage = 0;
                    mIsPaused = false;
                }

            }
        }
        doAction(mIManager->processEvents(mWindow->getEvent(), *mKeyHandler.get()));
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

	auto& Hero = GetHero();
    glm::vec2 CollisionOffset(0);
	const glm::vec2 Position = Hero.getPosition();
	static float radius = 0.24f;
    ImGui::SliderFloat("Collision radius", &radius, 0.05f, 1.f);
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
	mRenderer->getCamera().followEntity(GetHero(), 10.f, mDeltaTime);
}

void Game::doAction(Action const& a)
{
    auto& Hero = GetHero();
    ImGui::SliderFloat("Input Hero acceleration", &sInputAcceleration, 0, 10000);
	const float offset  = mDeltaTime * sInputAcceleration;
    
    switch (a)
    {
        case Action::Finish:
            mIsRunning = false;
			saveCurrentState();
            break;
        case Action::Pause:
            pause();
            break;
        case Action::StageFinished:
            stageFinished();
            break;
        case Action::CameraRotate:
            float x,y;
            mIManager->getMouseOffset(x, y);
            mRenderer->getCamera().processMouseMovement(x, y);
            break;
        default:
            break;
    }
    { // keyboard
        if (mKeyHandler->isPressed(SDL_SCANCODE_W))
            Hero.AddAcceleration(glm::vec2(0, -offset));
        if (mKeyHandler->isPressed(SDL_SCANCODE_D))
            Hero.AddAcceleration(glm::vec2(offset, 0));
        if (mKeyHandler->isPressed(SDL_SCANCODE_A))
            Hero.AddAcceleration(glm::vec2(-offset, 0));
        if (mKeyHandler->isPressed(SDL_SCANCODE_S))
            Hero.AddAcceleration(glm::vec2(0, offset));
		if (mKeyHandler->isPressed(SDL_SCANCODE_0))
			Hero.tryPlaceBomb();
    }
    { // joystick
        if (mKeyHandler->LeftJoystickIsActive()) {
            auto *joystick = mIManager->getJoystick();
            short x_move = SDL_JoystickGetAxis(joystick, 0);
            short y_move = SDL_JoystickGetAxis(joystick, 1);
            glm::vec2 normalizedJoystick(
                x_move / (float)MAX_JOYSTICK_VALUE,
                y_move / (float)MAX_JOYSTICK_VALUE
            );
            Hero.AddAcceleration(normalizedJoystick * offset);
        }
        if (mKeyHandler->JButtonIsPressed(SDL_CONTROLLER_BUTTON_A))
            pause();
        if (mKeyHandler->JButtonIsPressed(SDL_CONTROLLER_BUTTON_X))
			Hero.tryPlaceBomb();
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
        RESOURCES.loadShader("modelShader.vx.glsl", "modelShader.ft.glsl", "modelShader");
        RESOURCES.loadShader("skybox.vx.glsl", "skybox.ft.glsl", "skybox");
		RESOURCES.loadShader("sprite_p.vx.glsl", "sprite_p.ft.glsl", "sprite_p");
		RESOURCES.loadShader("sprite_quad.vx.glsl", "sprite_quad.ft.glsl", "sprite_quad");
		RESOURCES.loadShader("sprite_quad_brick.vx.glsl", "sprite_quad.ft.glsl", "sprite_quad_brick");
		RESOURCES.loadShader("sprite_quad_cloud.vx.glsl", "sprite_quad_cloud.ft.glsl", "sprite_quad_cloud");
		RESOURCES.loadShader("shadowShader.vx.glsl", "shadowShader.ft.glsl", "shadow");
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
        loadModels();
		mRenderer->getParticleManager()->init();
    }
    catch (CustomException &e)
    {
        std::cout << e.what() << std::endl;
        exit(42);
    }
}

void Game::loadModels()
{
    RESOURCES.loadModel("general/hero/model.fbx", "hero");
    RESOURCES.loadModel("general/bomb/model.fbx", "bomb", glm::vec3(1.3f), glm::vec3{0,-0.4f,0});

    RESOURCES.loadModel("map/first/ground/model.fbx", "ground");
    RESOURCES.loadModel("map/first/perimeterWall/model.fbx", "perimeterWall");
    RESOURCES.loadModel("map/first/unbreakableWall/model.fbx", "unbreakableWall", glm::vec3(1), glm::vec3(0),  glm::normalize(glm::vec3(1,0,0)), -90);
    RESOURCES.loadModel("map/first/breakableWall/model.fbx", "breakableWall", glm::vec3(0.9f));

    RESOURCES.loadModel("balloon/balloon.obj", "balloon");
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
    for (uint32_t i = 0; i < ARRAY_COUNT(directions); ++i)
        for (uint32_t j = 1; j <= span; ++j)
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

    auto &enemies = GetEnemies();
    std::for_each(enemies.begin(), enemies.end(), [this, hMin, hMax, vMin, vMax](MovingEntity *entity) {
        if (circle_box_collision(entity->getPosition() + glm::vec2(0.5f), .3f, hMin, hMax)
        || circle_box_collision(entity->getPosition() + glm::vec2(0.5f), .3f, vMin, vMax))
            entity->kill();
    });

    mRenderer->getParticleManager()->startDrawPS(brickPool[which], brickTransforms);
    mRenderer->getParticleManager()->startDrawPS(bombPool[which], fireTransforms);
	which = !which;
	mRenderer->getCamera().addShake(0.2f);
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


void       Game::stageFinished()
{
    int current_stage = CONFIGURATION.getChosenStage();
    CONFIGURATION.setChosenStage(current_stage < 3 ? current_stage + 1 : 0);
    Game::mReloadStage = true;
    if (mStageTimer > 4)
        mStageStartedTimer = getCurrentTime();
    mStageTimer = 3 - (getCurrentTime() - mStageStartedTimer);
}

Game *Game::get()
{
	return sInstance;
}

void	Game::tickAI(float deltaTime)
{
	if (ImGui::Button("Add balloon"))
	{
		GetBalloons().emplace_back(glm::vec2{9.5, 9.5});
	}
	recacheEnemies();
	for (auto& It : mBalloons)
		It.controller.tick(*It, deltaTime);
	for (auto& It : mBombs)
		It.controller.tick(*It, deltaTime);
}

std::vector<glm::mat4> Game::Filter(SquareType type)
{
	std::vector<glm::mat4> Result;
	auto& squares = mCollisionInfo.Squares;
	for (int i = 0; i < squares.size(); i++)
	{
		glm::vec2 Position {i % mCollisionInfo.width, i / mCollisionInfo.width};
		auto value = squares[i];
		if (value == type)
			Result.push_back(glm::translate(glm::mat4(1), glm::vec3{ Position.x + 0.5, 0, Position.y + 0.5}));
	}
	return Result;
}

void Game::recacheEnemies()
{
	mEnemies.clear();
	mBalloons.erase(std::remove_if(mBalloons.begin(), mBalloons.end(), [](const MovingEntity *balloon){
		return balloon->isDead();
	}), mBalloons.end());
	mBombs.erase(std::remove_if(mBombs.begin(), mBombs.end(), [](const Bomb *bomb){
		return bomb->isDead();
	}), mBombs.end());

	for (auto& It : mBalloons)
		mEnemies.push_back(It);
}

std::vector<glm::mat4> Game::GetWallTransforms() {
	return Filter(SquareType::Wall);
}

std::vector<glm::mat4> Game::GetBrickTransforms() {
	return Filter(SquareType::Brick);
}

std::vector<glm::mat4> Game::GetBombTransforms() {
	std::vector<glm::mat4> Result;
	for (Bomb* It : mBombs)
    {
		Result.push_back(It->getModelMatrix());
    }
	return Result;
}

std::vector<glm::mat4> Game::GetBonusTransforms() {
	return Filter(SquareType::Bonus);
}

Hero& Game::GetHero()
{
	return *mHero;
}

void Game::plantBomb(glm::vec2 position, int strength)
{
	mBombs.emplace_back(position, strength);
}

std::vector<MovingEntity*>& Game::GetEnemies()
{
	return mEnemies;
}

std::vector<Game::Balloon>& Game::GetBalloons()
{
	return mBalloons;
}
