#ifndef BOMBERMAN_Game_HPP
#define BOMBERMAN_Game_HPP
#include <memory>
#include <SDL_thread.h>

#include "LogicCore/LoadMap.h"
#include "GameWindow.hpp"
#include "GraphicCore/Renderer.hpp"
#include "InputManagement/InputManager.hpp"
#include "InputManagement/KeyboardHandler.hpp"
#include "AI/Agent.h"
#include "Entity/Bomb.h"
#include "LogicCore/Hero.h"

class Game
{
public:
    Game();
    ~Game();
    void start();
    void pause();
	static float getCurrentTime();
	static CollisionInfo& getCollisionInfo();
	void 		saveCurrentState(std::string fileName  = "ConfigureFile");
	void 		loadStateFromFile(std::string fileName  = "ConfigureFile");
	void 		applyWindowChange();
    void        stageFinished();
	void        onHeroDied();
	void        gameOver();
	void        cleanupOnStageChange();

    static bool    mReloadStage;
    static float   mStageTimer;
    static float   mStageStartedTimer;
    static bool    mIsRunning;
    static float   mDeltaTime;

    void        explosion(glm::ivec2 position, uint32_t span);

    static Game             *get();

	Game(Game const&) = delete;
    Game& operator=(Game const&) = delete;

private:
	void		resolveCollisions();
    void		doAction(Action const&);
    void		calcDeltaTime();
    void		loadResources();
    void        loadModels();
    std::shared_ptr<GameWindow>      mWindow;
    std::unique_ptr<Renderer>        mRenderer;
    std::unique_ptr<InputManager>    mIManager;
    std::unique_ptr<KeyboardHandler> mKeyHandler;

    static Uint64                    mTimeNow;
    static Uint64                    mTimeLast;
    static float                     mTimeCorrection;
	static CollisionInfo			 mCollisionInfo;
	static float					 sInputAcceleration;
	static Game					    *sInstance;

    bool                             mIsPaused;

	typedef Agent<MovingEntity, BalloonController>	BalloonAgent;
	typedef Agent<Bomb, BombController>				BombAgent;
public:
	void                        addEnemiesOnMap();
	std::vector<glm::mat4>		getWallTransforms();
	std::vector<glm::mat4>		getBrickTransforms();
	std::vector<glm::mat4>		getBombTransforms();
	glm::mat4					getPowerupTransform();
	glm::mat4					getExitTransform();

	std::vector<MovingEntity*>&	getEnemies();
	std::vector<BalloonAgent>&		getBalloons();

	Hero&						getHero();
	Hero::PowerupType			powerupTypeOnMap();
	bool						isExitActive();
	void						extractInfo();
	void						plantBomb(glm::vec2 position, int strength);
	void						tickAI(float deltaTime);

private:
	std::unique_ptr<Hero>			mHero;

	int buffer1[512] = { 0 };
	std::vector<BalloonAgent>		mBalloons;

	int buffer2[512] = { 0 };

	std::vector<BombAgent>			mBombs;

	int buffer3[512] = { 0 };

	std::vector<MovingEntity*>		mEnemies;

	glm::vec2						mPowerup;
	Hero::PowerupType				mPowerupType { Hero::PowerupType::PT_NONE };

	glm::vec2						mExit{ -1, -1 };

	std::vector<glm::mat4>		Filter(SquareType type);
	void						recacheEnemies();

	SDL_Thread					*moviePlayer;
};

#endif //BOMBERMAN_Renderer_HPP
