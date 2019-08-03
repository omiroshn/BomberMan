#include "Game.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "ResourceManagement/Model.hpp"
#include "Core.hpp"
#include "Entity/MovingEntity.h"
#include <tuple>
#include <functional>
#include "AI/AIController.h"
#include "LogicCore/Timer.h"
#include "LogicCore/TimerManager.h"
#include "Configure.hpp"

Uint64			Game::mTimeNow;
Uint64			Game::mTimeLast;
float			Game::mTimeCorrection;
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
        doAction(mIManager->processEvents(mWindow->getEvent(), *mKeyHandler.get()));
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
                mStageTimer = 200 - (getCurrentTime() - mStageStartedTimer);
                mWindow->ShowInGameMenu();
                mRenderer->getParticleManager()->update();
                if (index)
                {
                    const float TargetDelta = 0.0167f * (float)index;
					float ms = TargetDelta - mDeltaTime;
                    if (mDeltaTime < TargetDelta)
                        SDL_Delay(static_cast<Uint32>(ms * 1000));
                }
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
                    extractInfo();
                    if (mHero)
                    {
    					Hero::Stats info = mHero->getStats();
					    mHero = std::make_unique<Hero>(info);
                    }
                    else
                        mHero = std::make_unique<Hero>(CONFIGURATION.getStats());
                    mStageStartedTimer = getCurrentTime();
                    mReloadStage = 0;
                    mIsPaused = false;
                }

            }
        }
        mWindow->update();
    }
    CONFIGURATION.serialise();
}

void Game::pause()
{
    mIsPaused = !mIsPaused;
    mWindow.get()->PauseGame(mIsPaused);
}

float Game::getCurrentTime()
{
	return mTimeNow / static_cast<float>(SDL_GetPerformanceFrequency()) - mTimeCorrection;
}

CollisionInfo& Game::getCollisionInfo()
{
	return mCollisionInfo;
}

bool circle_circle_collision(glm::vec2 a, float radiusA, glm::vec2 b, float radiusB)
{
    glm::vec2 CollisionDirection = a - b;
    float distance = glm::length(CollisionDirection);
    return distance < (radiusA + radiusB);
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
    static float CollisionResolveMultiplier = 350.f;
    ImGui::SliderFloat("CollisionResolveMultiplier", &CollisionResolveMultiplier, 100, 1000);

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

	auto& Hero = getHero();
    glm::vec2 CollisionOffset(0);
    const glm::vec2 Position = Hero.getPosition();
    static float radius = 0.24f;
    ImGui::SliderFloat("Collision radius", &radius, 0.05f, 1.f);
    if (mPowerupType != Hero::PowerupType::PT_NONE
    && circle_circle_collision(Position, radius, mPowerup, radius))
    {
        Hero.applyPowerup(mPowerupType);
        mPowerupType = Hero::PowerupType::PT_NONE;
    }

    for (size_t i = 0; i < ARRAY_COUNT(offsets); i++)
    {
        glm::vec2 ProbePoint = Position + offsets[i];
        bool inObstacle = mCollisionInfo[ProbePoint] != SquareType::EmptySquare;
        if (!inObstacle)
            continue;
        glm::vec2 ResolutionOffset;
        if (circle_box_collision(Position, radius, glm::floor(ProbePoint), glm::ceil(ProbePoint), &ResolutionOffset))
            CollisionOffset += ResolutionOffset;
    }

    for (MovingEntity* It : mBalloons)
    {
        if (circle_circle_collision(Hero.getPosition(), radius, It->getPosition(), radius))
            Hero.kill();
        glm::vec2 ProbePoint = Position;
        if (mCollisionInfo[ProbePoint] == SquareType::Bomb)
        {
            glm::vec2 center = glm::floor(ProbePoint) + glm::vec2(0.5f);
            It->AddAcceleration(glm::normalize(ProbePoint - center) * CollisionResolveMultiplier);
        }
    }

    Hero.move(CollisionOffset / 4.f);
    Hero.AddAcceleration(CollisionOffset * CollisionResolveMultiplier);
	mRenderer->getCamera().followEntity(getHero(), 10.f, mDeltaTime);
}

void Game::doAction(Action const& a)
{
    auto& Hero = getHero();
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
        case Action::HeroDied:
            onHeroDied();
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
    static float SpeedOfTime = 1.f;
    ImGui::SliderFloat("Spee of time", &SpeedOfTime, 0.0001f, 2.f);
    mTimeLast = mTimeNow;
    mTimeNow = SDL_GetPerformanceCounter();

	mDeltaTime = (mTimeNow - mTimeLast) / static_cast<float>(SDL_GetPerformanceFrequency());
	if (mDeltaTime > 1.f)
		mDeltaTime = 0.016f;
    if (SpeedOfTime != 1.f)
    {
        float newDelta = mDeltaTime * SpeedOfTime;
        mTimeCorrection += mDeltaTime - newDelta;
        mDeltaTime = newDelta;
    }
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
		RESOURCES.loadShader("sparks.vx.glsl", "sparks.ft.glsl", "sparks");
		RESOURCES.loadShader("gui.vx.glsl", "gui.ft.glsl", "gui");
        RESOURCES.loadTexture("block.png", "block");
        RESOURCES.loadTexture("brickwall.png", "brickwall");
        RESOURCES.loadTexture("sky.png", "sky");
        RESOURCES.loadTexture("locked.png", "locked");
        RESOURCES.loadTexture("unlocked0.png", "unlocked0");
        RESOURCES.loadTexture("unlocked0.png", "unlocked1");
        RESOURCES.loadTexture("unlocked0.png", "unlocked2");
        RESOURCES.loadTexture("unlocked0.png", "unlocked3");
        RESOURCES.loadTexture("container.jpg", "container");
        RESOURCES.loadTexture("awesomeface.png", "face");
		RESOURCES.loadTexture("flame-fire.png", "flame-fire");
		RESOURCES.loadTexture("cloud_trans.jpg", "cloud_trans");
		RESOURCES.loadTexture("explode.png", "explosion_tmap_2");
		RESOURCES.loadTexture("sparks.jpg", "sparks");
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

    // powerups placeholder, please do something about this!!!!!!!!!!!!!!!!!!!!!
    RESOURCES.loadModel("general/powerup/model.dae", "bonus_bombs", glm::vec3(0.5f), glm::vec3(0), glm::vec3(0,1,0), 180.f);
    RESOURCES.loadModel("general/powerup/model.dae", "bonus_flames", glm::vec3(0.5f), glm::vec3(0), glm::vec3(0,1,0), 180.f);
    RESOURCES.loadModel("general/powerup/model.dae", "bonus_speed", glm::vec3(0.5f), glm::vec3(0), glm::vec3(0,1,0), 180.f);
    RESOURCES.loadModel("general/powerup/model.dae", "bonus_wallpass", glm::vec3(0.5f), glm::vec3(0), glm::vec3(0,1,0), 180.f);
    RESOURCES.loadModel("general/powerup/model.dae", "bonus_detonator", glm::vec3(0.5f), glm::vec3(0), glm::vec3(0,1,0), 180.f);
    RESOURCES.loadModel("general/powerup/model.dae", "bonus_bombpass", glm::vec3(0.5f), glm::vec3(0), glm::vec3(0,1,0), 180.f);
    RESOURCES.loadModel("general/powerup/model.dae", "bonus_flamepass", glm::vec3(0.5f), glm::vec3(0), glm::vec3(0,1,0), 180.f);
    RESOURCES.loadModel("general/powerup/model.dae", "bonus_mystery", glm::vec3(0.5f), glm::vec3(0), glm::vec3(0,1,0), 180.f);
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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

    struct Overlaper
    {
        glm::vec2 point[4];
        Overlaper(glm::ivec2 position) : point{
            position,
            position + glm::ivec2(1),
            position,
            position + glm::ivec2(1)
        } {}
        glm::ivec2 operator[](int i) {
            return point[i];
        };
    };

    std::vector<Overlaper> overlaps;

    glm::vec2 centerPosition = glm::vec2(position) + glm::vec2(0.5f);

    std::vector<glm::mat4> fireTransforms;
    std::vector<glm::mat4> brickTransforms;
    fireTransforms.push_back(glm::translate(glm::mat4(1), glm::vec3(centerPosition.x, 0, centerPosition.y)));

    std::vector<SquareType*> deferedBricks;

std::function<void (glm::vec2)> chainReaction = [&] (glm::vec2 center) {
    Overlaper minMax(center);
    for (uint32_t i = 0; i < ARRAY_COUNT(directions); ++i)
        for (uint32_t j = 1; j <= span; ++j)
        {
            glm::vec2 testPosition = center + (float)j * directions[i];
            auto& type = mCollisionInfo[testPosition];
            if (type == SquareType::Wall)
                break;

            fireTransforms.push_back(glm::translate(glm::mat4(1), glm::vec3(testPosition.x, 0, testPosition. y)));
            minMax[i] += directions[i];

            if (type == SquareType::Brick)
            {
                brickTransforms.push_back(glm::translate(glm::mat4(1), glm::vec3(testPosition.x, 0, testPosition. y)));
                deferedBricks.push_back(&type);
                break;
            }

            if (type == SquareType::Bomb)
            {
                type = SquareType::EmptySquare;
                chainReaction(testPosition);
                for (Entity* It : mBombs)
                    if (testPosition == It->getPosition())
                        It->kill();
            }
        }
    overlaps.push_back(minMax);
};
    chainReaction(centerPosition);
    for (auto It : deferedBricks)
        *It = SquareType::EmptySquare;

    auto &enemies = getEnemies();
    auto &hero = getHero();

    std::for_each(overlaps.begin(), overlaps.end(), [&enemies, &hero](Overlaper &overlap) {
        auto hMin = overlap[0];
        auto hMax = overlap[1];
        auto vMin = overlap[2];
        auto vMax = overlap[3];

        std::for_each(enemies.begin(), enemies.end(), [hMin, hMax, vMin, vMax](MovingEntity *entity) {
            if (circle_box_collision(entity->getPosition() + glm::vec2(0.5f), .3f, hMin, hMax) || circle_box_collision(entity->getPosition() + glm::vec2(0.5f), .3f, vMin, vMax))
                entity->kill();
        });
        if (circle_box_collision(hero.getPosition() + glm::vec2(0.5f), .3f, hMin, hMax) || circle_box_collision(hero.getPosition() + glm::vec2(0.5f), .3f, vMin, vMax))
            hero.kill();
    });

    mRenderer->getParticleManager()->startDrawPS(brickPool[which], brickTransforms);
    mRenderer->getParticleManager()->startDrawPS(bombPool[which], fireTransforms);
	which = !which;
	mRenderer->getCamera().addShake(0.05f);
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
    CONFIGURATION.setBestLevelAchieved(current_stage);
    CONFIGURATION.setChosenStage(current_stage < 3 ? current_stage + 1 : 0);
    Game::mReloadStage = true;
    if (mStageTimer > 4)
        mStageStartedTimer = getCurrentTime();
    mStageTimer = 3 - (getCurrentTime() - mStageStartedTimer);
    cleanupOnStageChange();
}

void Game::onHeroDied()
{
    if (mReloadStage)
        return;
    if (CONFIGURATION.getLives() == 1)
        gameOver();
    else
        CONFIGURATION.setLives(CONFIGURATION.getLives() - 1);
    if (mStageTimer > 4)
        mStageStartedTimer = getCurrentTime();
    mStageTimer = 3 - (getCurrentTime() - mStageStartedTimer);
    Game::mReloadStage = true;
    cleanupOnStageChange();
}

void Game::gameOver()
{
    CONFIGURATION.setLives(DefaultLives);
    CONFIGURATION.setScore(DefaultScore);
    CONFIGURATION.setChosenStage(DefaultChosenStage);
    pause();
}

void  Game::cleanupOnStageChange()
{
    mBalloons.clear();
    mBombs.clear();
}

Game *Game::get()
{
	return sInstance;
}

void	Game::tickAI(float deltaTime)
{
	if (ImGui::Button("Add balloon"))
	{
		getBalloons().emplace_back(glm::vec2{9.5, 9.5});
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
	for (size_t i = 0; i < squares.size(); i++)
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

std::vector<glm::mat4> Game::getWallTransforms() {
	return Filter(SquareType::Wall);
}

std::vector<glm::mat4> Game::getBrickTransforms() {
	return Filter(SquareType::Brick);
}

std::vector<glm::mat4> Game::getBombTransforms() {
	std::vector<glm::mat4> Result;
	for (Bomb* It : mBombs)
    {
		Result.push_back(It->getModelMatrix());
    }
	return Result;
}

glm::mat4 Game::getPowerupTransform() {
    return glm::translate(glm::mat4(1.f), glm::vec3{mPowerup.x, 0, mPowerup.y});
}

Hero::PowerupType Game::powerupTypeOnMap() {
    return mPowerupType;
}

Hero& Game::getHero()
{
	return *mHero;
}

void Game::plantBomb(glm::vec2 position, int strength)
{
	mBombs.emplace_back(position, strength);
}

std::vector<MovingEntity*>& Game::getEnemies()
{
	return mEnemies;
}

std::vector<Game::Balloon>& Game::getBalloons()
{
	return mBalloons;
}

void Game::extractInfo()
{
    for (size_t i = 0; i < mCollisionInfo.Squares.size(); i++)
    {
        auto& It = mCollisionInfo.Squares[i];
        if (It >= SquareType::Powerup_Bombs && It <= SquareType::Powerup_Mystery)
        {
            mPowerupType = (Hero::PowerupType)((int)It - (int)SquareType::Powerup_Bombs);
            mPowerup = glm::vec2 { i % mCollisionInfo.width, i / mCollisionInfo.width };
            mPowerup += glm::vec2(0.5f);
            It = SquareType::Brick;
        }
        if (It == SquareType::Exit)
        {
            mExit = glm::vec2 { i % mCollisionInfo.width, i / mCollisionInfo.width };
            mExit += glm::vec2(0.5f);
            It = SquareType::Brick;
        }
    }
}