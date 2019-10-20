#include "Game.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "ResourceManagement/MusicPlayer.hpp"
#include "ResourceManagement/Model.hpp"
#include "ResourceManagement/Texture.hpp"
#include "Core.hpp"
#include "Entity/MovingEntity.h"
#include "AI/AIController.h"
#include "Configure.hpp"
#include "ResourceManagement/FfmpegPlayer.hpp"

#include <array>
#include <functional>
#include <thread>
#include "stb_image.h"

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
    sInstance = this;

	mTimeNow = SDL_GetPerformanceCounter();

	loadStateFromFile();

	mWindow = std::make_shared<GameWindow>(CONFIGURATION.getWidth(), CONFIGURATION.getHeight(), cWindowName);
	mWindow->setFullScreen(CONFIGURATION.getWindowed());
	CONFIGURATION.setObservableWindow(mWindow);

	mRenderer = std::make_unique<Renderer>();
    mIManager = std::make_unique<InputManager>();
    mKeyHandler = std::make_unique<KeyboardHandler>();

	RESOURCES.loadShader("sprite_p.vx.glsl", "sprite_p.ft.glsl", "sprite_p");
	RESOURCES.loadShader("sprite_quad_brick.vx.glsl", "sprite_quad.ft.glsl", "sprite_quad_brick");
	RESOURCES.loadTexture("flame-fire.png", "flame-fire");
	RESOURCES.loadTexture("brickwall.png", "brickwall");
	RESOURCES.loadTexture("container.jpg", "container");
	mRenderer->getParticleManager()->init();

    loadResources();
	FFMPEG.init(mWindow);
	FFMPEG.playVideo("intro.mp4");
	MUSIC_PLAYER.initLoad();
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

    SDL_GL_MakeCurrent(mWindow->getSDLWindow(), mWindow->getSDLGLContext());
    SDL_SetWindowInputFocus(mWindow->getSDLWindow());
	RESOURCES.endLoading();
	
	// sync files here
    while (mIsRunning)
    {
        mIManager->processEvents(this, *mKeyHandler.get());
        handleInput();
        mWindow->tickGui();
        mWindow->getSize(width, height);
        calcDeltaTime();
        mRenderer->updateSize(width, height);
        CONFIGURATION.setHeight(height);
        CONFIGURATION.setWidth(width);
        if (!mWindow.get()->IsGameRunning())
        {
            mWindow.get()->ShowStartingMenu();
			MUSIC_PLAYER.playMusicInfinity("tango");
        }
        else
        {
            if (mapLoader.MapIsLoaded() && !mReloadStage)
            {
                tickAI(mDeltaTime);
                MovingEntity::debugMovement();
                Tickable::tickTickables(mDeltaTime);
                resolveCollisions();
                mRenderer->getParticleManager()->update();
                mRenderer->getCamera().followEntity(getHero(), 10.f, mDeltaTime);
                mRenderer->draw(*this);
                if (mHero && !mHero->isDead())
                    mStageTimer = 200 - (getCurrentTime() - mStageStartedTimer);
#if DEBUG
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
#endif
                mWindow->ShowInGameMenu();
                if (mHero && mHero->isDead())
                {
                    if (mStageTimer < 2 && mStageTimer >= 0)
                        mReloadStage = true;
                    else
                    {
                        if (mStageTimer > 4)
                            mStageStartedTimer = getCurrentTime();
                        mStageTimer = 4 - (getCurrentTime() - mStageStartedTimer);
                    }
                }
            }
            else
            {
                if (CONFIGURATION.getLives() == 0)
                    CONFIGURATION.setChosenStage(1);
                if (mHero && mHero->isDead())
                {
                    getHero().getAnimation().setTime(0);
                }
                if (mReloadStage && mStageTimer > 1)
                {
                    if (mStageTimer < 2)
                        mWindow->ShowBetweenStageScreen();
                    mIsPaused = false;
                    mStageTimer = 4 - (getCurrentTime() - mStageStartedTimer);
                }
                else if (mStageTimer < 2)
                {
                    mapLoader.cleanMapForRendering();
                    mCollisionInfo.Squares.clear();
                    mCollisionInfo = mapLoader.GetMap(CONFIGURATION.getChosenStage() < 4 ? CONFIGURATION.getChosenStage() : 3);
                    extractInfo();
                    addEnemiesOnMap();
                    if (mHero)
                    {
    					Hero::Stats info = mHero->mStats;
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

void Game::handleInput()
{
    const float offset = mDeltaTime * sInputAcceleration;
    if (mHero && !mHero->isDead())
    {
        int keybinding = CONFIGURATION.getKeyBindVolume();
        if (keybinding == 0)
        {
            if (mKeyHandler->isPressed(SDL_SCANCODE_UP))
                mHero->AddAcceleration(glm::vec2(0, -offset));
            if (mKeyHandler->isPressed(SDL_SCANCODE_RIGHT))
                mHero->AddAcceleration(glm::vec2(offset, 0));
            if (mKeyHandler->isPressed(SDL_SCANCODE_LEFT))
                mHero->AddAcceleration(glm::vec2(-offset, 0));
            if (mKeyHandler->isPressed(SDL_SCANCODE_DOWN))
                mHero->AddAcceleration(glm::vec2(0, offset));
        }
        else if (keybinding == 1)
        {
            if (mKeyHandler->isPressed(SDL_SCANCODE_W))
                mHero->AddAcceleration(glm::vec2(0, -offset));
            if (mKeyHandler->isPressed(SDL_SCANCODE_D))
                mHero->AddAcceleration(glm::vec2(offset, 0));
            if (mKeyHandler->isPressed(SDL_SCANCODE_A))
                mHero->AddAcceleration(glm::vec2(-offset, 0));
            if (mKeyHandler->isPressed(SDL_SCANCODE_S))
                mHero->AddAcceleration(glm::vec2(0, offset));
        }
        else if (keybinding == 2)
        {
            if (mKeyHandler->isPressed(SDL_SCANCODE_K))
                mHero->AddAcceleration(glm::vec2(0, -offset));
            if (mKeyHandler->isPressed(SDL_SCANCODE_L))
                mHero->AddAcceleration(glm::vec2(offset, 0));
            if (mKeyHandler->isPressed(SDL_SCANCODE_H))
                mHero->AddAcceleration(glm::vec2(-offset, 0));
            if (mKeyHandler->isPressed(SDL_SCANCODE_J))
                mHero->AddAcceleration(glm::vec2(0, offset));
        }
        if (auto *joystick = mIManager->getJoystick())
        {
            short x_move = SDL_JoystickGetAxis(joystick, 0);
            short y_move = SDL_JoystickGetAxis(joystick, 1);

            // this is mandatory
            if (x_move < JOYSTICK_DEAD_ZONE && x_move > -JOYSTICK_DEAD_ZONE)
                x_move = 0;
            if (y_move < JOYSTICK_DEAD_ZONE && y_move > -JOYSTICK_DEAD_ZONE)
                y_move = 0;
            //

            glm::vec2 normalizedJoystick(
                x_move / (float)MAX_JOYSTICK_VALUE,
                y_move / (float)MAX_JOYSTICK_VALUE);
            mHero->AddAcceleration(normalizedJoystick * offset);
        }
    }
    if (auto *joystick = mIManager->getJoystick())
    {
        short x_move = SDL_JoystickGetAxis(joystick, 0);
        short y_move = SDL_JoystickGetAxis(joystick, 1);

        ImGuiIO &io = ImGui::GetIO();
        io.NavInputs[ImGuiNavInput_DpadLeft] = x_move < -JOYSTICK_DEAD_ZONE;
        io.NavInputs[ImGuiNavInput_DpadRight] = x_move > JOYSTICK_DEAD_ZONE;
        io.NavInputs[ImGuiNavInput_DpadUp] = y_move < -JOYSTICK_DEAD_ZONE;
        io.NavInputs[ImGuiNavInput_DpadDown] = y_move > JOYSTICK_DEAD_ZONE;
    }
}

void Game::resolveCollisions()
{
    static float CollisionResolveMultiplier = 300.f;
    static float radius = 0.24f;
#if DEBUG
    ImGui::SliderFloat("CollisionResolveMultiplier", &CollisionResolveMultiplier, 100, 1000);
    ImGui::SliderFloat("Collision radius", &radius, 0.05f, 1.f);
#endif

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

	// pick up item
    if (mPowerupType != Hero::PowerupType::PT_NONE
    && circle_circle_collision(Position, radius, mPowerup, radius))
    {
        Hero.applyPowerup(mPowerupType);
        MUSIC_PLAYER.playSound("bonus");
        mPowerupType = Hero::PowerupType::PT_NONE;
    }

    for (size_t i = 0; i < ARRAY_COUNT(offsets); i++)
    {
        glm::vec2 ProbePoint = Position + offsets[i];
		SquareType inObstacle = mCollisionInfo[ProbePoint];
        if ((inObstacle == SquareType::EmptySquare)
        || (Hero.mStats.wallpass && inObstacle == SquareType::Brick)
        || (Hero.mStats.bombpass && inObstacle == SquareType::Bomb))
			continue;
        glm::vec2 ResolutionOffset;
        if (circle_box_collision(Position, radius, glm::floor(ProbePoint), glm::ceil(ProbePoint), &ResolutionOffset))
            CollisionOffset += ResolutionOffset;

		if (inObstacle == SquareType::Bomb)
			CollisionOffset /= 2.f;
    }

    for (MovingEntity* It : mEnemies)
    {
        if (It->isDead())
            continue;
        if (circle_circle_collision(Hero.getPosition(), radius, It->getPosition(), radius))
            Hero.kill();
		glm::vec2 ProbePoint = It->getPosition();
        if (mCollisionInfo[ProbePoint] == SquareType::Bomb)
        {
            glm::vec2 center = glm::floor(ProbePoint) + glm::vec2(0.5f);
            It->AddAcceleration(glm::normalize(ProbePoint - center) * CollisionResolveMultiplier);
        }
    }

    Hero.move(CollisionOffset / 8.f);
    Hero.AddAcceleration(CollisionOffset * CollisionResolveMultiplier);

	if (isExitActive() 
    &&  circle_circle_collision(Position, radius / 2.f, mExit, radius / 2.f))
	{
        FFMPEG.playVideo("betweenstages.mp4");
		stageFinished();
	}
}

void Game::doAction(Action a)
{
    switch (a)
    {
        case Action::Finish:
            requestExit();
			saveCurrentState();
            break;
        case Action::Explosion:
            if (mHero)
                mHero->tryPlaceBomb();
            break;
        case Action::Pause:
            pause();
            break;
        case Action::KillAll:
            for (auto& It : mEnemies)
                It->kill();
            break;
#if DEBUG
        case Action::CameraRotate:
            float x,y;
            mIManager->getMouseOffset(x, y);
            mRenderer->getCamera().processMouseMovement(x, y);
            break;
#endif
        default:
            break;
    }
}

void Game::calcDeltaTime()
{
    static float SpeedOfTime = 1.f;
#if DEBUG
    ImGui::SliderFloat("Speed of time", &SpeedOfTime, 0.0001f, 2.f);
#endif
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
#if DEBUG
	ImGui::Text("Current time: %f", getCurrentTime());
	ImGui::Text("Delta time: %f", mDeltaTime);
#endif
}

void Game::loadResources()
{
    try
    {
        RESOURCES.loadShader("modelShader.vx.glsl", "modelShader.ft.glsl", "modelShader");
        RESOURCES.loadShader("animatedModelShader.vx.glsl", "modelShader.ft.glsl", "animatedModelShader");
        RESOURCES.loadShader("skybox.vx.glsl", "skybox.ft.glsl", "skybox");
		RESOURCES.loadShader("shadowShader.vx.glsl", "shadowShader.ft.glsl", "shadow");
		RESOURCES.loadShader("animatedShadowShader.vx.glsl", "shadowShader.ft.glsl", "animatedShadow");
		RESOURCES.loadShader("sparks.vx.glsl", "sparks.ft.glsl", "sparks");
		RESOURCES.loadShader("gui.vx.glsl", "gui.ft.glsl", "gui");
        RESOURCES.loadTexture("block.png", "block");
        RESOURCES.loadTexture("wallpass.png", "wallpass");
        RESOURCES.loadTexture("sky.png", "sky");
        RESOURCES.loadTexture("locked.png", "locked");
        RESOURCES.loadTexture("unlocked0.png", "unlocked0");
        RESOURCES.loadTexture("unlocked0.png", "unlocked1");
        RESOURCES.loadTexture("unlocked0.png", "unlocked2");
        RESOURCES.loadTexture("unlocked0.png", "unlocked3");
        RESOURCES.loadTexture("victory.png", "face");
		RESOURCES.loadTexture("cloud_trans.jpg", "cloud_trans");
		RESOURCES.loadTexture("explode.png", "explosion_tmap_2");
		RESOURCES.loadTexture("sparks.jpg", "sparks");
        RESOURCES.loadSkybox("defaultSkybox");
        RESOURCES.loadSkybox("blue");
        RESOURCES.loadSkybox("lightblue");
        loadModels();
    }
    catch (CustomException &e)
    {
        std::cout << e.what() << std::endl;
		requestExit();
    }
}

void Game::loadModels()
{
    RESOURCES.loadModel("general/hero/model.fbx", "hero", glm::vec3(1.f), glm::vec3{0,0,0}, glm::vec3(0,1,0), 0.f, 1.f);
    RESOURCES.loadModel("general/bomb/model.fbx", "bomb", glm::vec3(1.3f), glm::vec3{0,0.3,0}, glm::vec3(1,0,0), -90.f, 1.f);

    // powerups placeholder, please do something about this!!!!!!!!!!!!!!!!!!!!!
    RESOURCES.loadModel("general/powerup/model.dae", "bonus", glm::vec3(0.5f), glm::vec3(0), glm::vec3(0,1,0), 180.f);
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    RESOURCES.loadModel("map/first/ground/model.fbx", "ground");
    RESOURCES.loadModel("map/first/perimeterWall/model.fbx", "perimeterWall");
    RESOURCES.loadModel("map/first/unbreakableWall/model.fbx", "unbreakableWall", glm::vec3(1), glm::vec3(0),  glm::normalize(glm::vec3(1,0,0)), -90);
    RESOURCES.loadModel("map/first/breakableWall/model.fbx", "breakableWall", glm::vec3(0.9f));

    RESOURCES.loadModel("general/monster/model.fbx", "enemy1");
    RESOURCES.loadModel("general/villain/model.fbx", "enemy2");
    RESOURCES.loadModel("general/troll/model.fbx", "enemy3", glm::vec3(0.025f), glm::vec3(0.f, -150.f, 0.f));

    RESOURCES.loadModel("general/gate_active/model.fbx", "gate1", glm::vec3(0.9f), glm::vec3(0.f, -45.f, 0.f), glm::vec3(1,0,0), 270.f);
    RESOURCES.loadModel("general/gate_inactive/model.fbx", "gate2", glm::vec3(0.9f), glm::vec3(0.f, -45.f, 0.f), glm::vec3(1,0,0), 270.f);
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

	typedef std::array<glm::vec2, 4> Overlaper;

    auto MakeOverlaper = [](glm::ivec2 position) {
        return Overlaper{
                {position,
                position + glm::ivec2(1),
                position,
                position + glm::ivec2(1)}
        };
    };
    std::vector<Overlaper> overlaps;

    glm::vec2 centerPosition = glm::vec2(position) + glm::vec2(0.5f);

    std::vector<glm::mat4> fireTransforms;
    std::vector<glm::mat4> brickTransforms;
    fireTransforms.push_back(glm::translate(glm::mat4(1), glm::vec3(centerPosition.x, 0, centerPosition.y)));

    std::vector<SquareType*> deferedBricks;

std::function<void (glm::vec2, uint32_t)> chainReaction = [&] (glm::vec2 center, uint32_t explosionStrength) {
    glm::vec2 directions[] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    Overlaper minMax = MakeOverlaper(center);
    for (uint32_t i = 0; i < ARRAY_COUNT(directions); ++i)
        for (uint32_t j = 1; j <= explosionStrength; ++j)
        {
            glm::vec2 testPosition = center + (float)j * directions[i];
            auto& type = mCollisionInfo[testPosition];

			if (type == SquareType::EmptySquare)
			{
				fireTransforms.push_back(glm::translate(glm::mat4(1), glm::vec3(testPosition.x, 0, testPosition. y)));
				minMax[i] += directions[i];
			}
            else if (type == SquareType::Brick)
            {
                brickTransforms.push_back(glm::translate(glm::mat4(1), glm::vec3(testPosition.x, 0, testPosition. y)));
                deferedBricks.push_back(&type);
                break;
            }
            else if (type == SquareType::Bomb)
            {
                type = SquareType::EmptySquare;
                for (Bomb* It : mBombs)
                    if (testPosition == It->getPosition())
                    {
                        It->kill();
                        chainReaction(testPosition, It->mExplosionStrength);
                        break;
                    }
            }
			else if (type == SquareType::Wall)
			{
                break;
			}
        }
    overlaps.push_back(minMax);
};
    chainReaction(centerPosition, span);
    for (auto It : deferedBricks)
    {
        *It = SquareType::EmptySquare;
        CONFIGURATION.setScore(CONFIGURATION.getScore() + 5);
    }

    auto &enemies = getEnemies();
    auto &hero = getHero();

    std::for_each(overlaps.begin(), overlaps.end(), [&enemies, &hero](Overlaper &overlap) {
        auto hMin = overlap[0];
        auto hMax = overlap[1];
        auto vMin = overlap[2];
        auto vMax = overlap[3];

        std::for_each(enemies.begin(), enemies.end(), [hMin, hMax, vMin, vMax](MovingEntity *entity) {
            if (circle_box_collision(entity->getPosition(), .4f, hMin, hMax)
             || circle_box_collision(entity->getPosition(), .4f, vMin, vMax))
                {
                    entity->kill();
                    CONFIGURATION.setScore(CONFIGURATION.getScore() + 10);
                }
        });

		if ((circle_box_collision(hero.getPosition(), .1f, hMin, hMax)
		  || circle_box_collision(hero.getPosition(), .1f, vMin, vMax))
		  && !hero.mStats.flamepass)
		{
            hero.kill();
            CONFIGURATION.setScore(DefaultScore);
		}
    });

    mRenderer->getParticleManager()->startDrawPS(brickPool[which], brickTransforms);
    mRenderer->getParticleManager()->startDrawPS(bombPool[which], fireTransforms);
	mRenderer->getCamera().addShake(0.05f);
	MUSIC_PLAYER.playSound("explosion");
	which = !which;
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
    if (!mHero->isDead())
    {
        CONFIGURATION.setChosenStage(current_stage < 3 ? current_stage + 1 : 0);
        if (current_stage == 3)
        {
            MUSIC_PLAYER.playSound("bomberman");
            FFMPEG.playVideo("victory.mp4");
        }
    }
    Game::mReloadStage = true;
    if (mStageTimer > 4)
        mStageStartedTimer = getCurrentTime();
    mStageTimer = 3 - (getCurrentTime() - mStageStartedTimer);
    cleanupOnStageChange();
}

void Game::onHeroDied()
{
    if (mStageTimer < 3)
    {
        stageFinished();
    }
    if (CONFIGURATION.getLives() == 1)
        gameOver();
    else
        CONFIGURATION.setLives(CONFIGURATION.getLives() - 1);
}

void Game::gameOver()
{
    CONFIGURATION.setLives(DefaultLives);
    CONFIGURATION.setScore(DefaultScore);
    CONFIGURATION.setChosenStage(DefaultChosenStage);
    CONFIGURATION.setBestLevelAchieved(DefaultBestLevelAchieved);
    mStageTimer = 200;
    mStageStartedTimer = getCurrentTime();
    pause();
}

void  Game::cleanupOnStageChange()
{
    mBalloons.clear();
    mOnils.clear();
    mOvapes.clear();
    mBombs.clear();
    mRenderer->getParticleManager()->clear();
}

void Game::requestExit()
{
	mIsRunning = false;
}

Game *Game::get()
{
	return sInstance;
}

void	Game::tickAI(float deltaTime)
{
#if DEBUG
	if (ImGui::Button("Kill all"))
	{
		for (auto& It : mEnemies)
			It->kill();
	}
#endif

	recacheEnemies();

	for (auto& It : mBombs)
		It.controller.tick(*It, deltaTime);
	for (auto& It : mBalloons)
		It.controller.tick(*It, deltaTime);
	for (auto& It : mOnils)
		It.controller.tick(*It, deltaTime);
	for (auto& It : mOvapes)
		It.controller.tick(*It, deltaTime);

	if (mHero->isDeadForAwhile())
		onHeroDied();
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
	mBalloons.erase(std::remove_if(mBalloons.begin(), mBalloons.end(), [](const MovingEntity *balloon){
		return balloon->isDeadForAwhile();
	}), mBalloons.end());
	mOnils.erase(std::remove_if(mOnils.begin(), mOnils.end(), [](const MovingEntity *onil){
		return onil->isDeadForAwhile();
	}), mOnils.end());
	mOvapes.erase(std::remove_if(mOvapes.begin(), mOvapes.end(), [](const MovingEntity *ovapes){
		return ovapes->isDeadForAwhile();
	}), mOvapes.end());

	mBombs.erase(std::remove_if(mBombs.begin(), mBombs.end(), [](const Bomb *bomb){
		return bomb->isDead();
	}), mBombs.end());

	mEnemies.clear();
	for (auto& It : mBalloons)
		mEnemies.push_back(It);
	for (auto& It : mOnils)
		mEnemies.push_back(It);
	for (auto& It : mOvapes)
		mEnemies.push_back(It);
}

void  Game::addEnemiesOnMap()
{
    cleanupOnStageChange();
    for (size_t i = 20; i < mCollisionInfo.Squares.size() - 20; i++)
    {
        auto& It = mCollisionInfo.Squares[i];
        if (It >= SquareType::Brick && rand() % 9 == 0)
        {
            int type = rand() % 3;
            switch (type)
            {
                case 0:{
                    if (mCollisionInfo.Squares[i + 1] == SquareType::EmptySquare)
                        mBalloons.emplace_back(glm::vec2{(i + 1) % mCollisionInfo.width + 0.5f, i / mCollisionInfo.width + 0.5f});
                }
                case 1:{
                    if (mCollisionInfo.Squares[i + 1] == SquareType::EmptySquare)
                        mOnils.emplace_back(glm::vec2{(i + 1) % mCollisionInfo.width + 0.5f, i / mCollisionInfo.width + 0.5f});
                }
                case 2:{
                    if (mCollisionInfo.Squares[i + 1] == SquareType::EmptySquare)
                        mOvapes.emplace_back(glm::vec2{(i + 1) % mCollisionInfo.width + 0.5f, i / mCollisionInfo.width + 0.5f});
                }
            }
        }
    }
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

glm::mat4 Game::getExitTransform()
{
	return glm::translate(glm::mat4(1), glm::vec3(mExit.x, 0, mExit.y));
}

Hero::PowerupType Game::powerupTypeOnMap()
{
    return mPowerupType;
}

bool Game::isExitActive()
{
	return mEnemies.empty();
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

void Game::extractInfo()
{
    for (size_t i = 0; i < mCollisionInfo.Squares.size(); i++)
    {
        auto& It = mCollisionInfo.Squares[i];
        if (It >= SquareType::Powerup_Bombs && It <= SquareType::Powerup_Flamepass)
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
