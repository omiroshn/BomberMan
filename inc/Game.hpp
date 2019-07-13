#ifndef BOMBERMAN_Game_HPP
#define BOMBERMAN_Game_HPP
#include <memory>
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

    void        explosion(glm::ivec2 position, uint32_t span);

    static Game             *get();
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
    static float                     mDeltaTime;
    static float                     mTimeCorrection;
	static CollisionInfo			 mCollisionInfo;
    bool                             mIsPaused;
	static float					 sInputAcceleration;
	static Game					    *sInstance;

	// Map for rendering
	typedef Agent<MovingEntity, BalloonController>	Balloon;
	typedef Agent<Bomb, BombSM>						BombAgent;
public:
	std::vector<glm::mat4>		GetWallTransforms();
	std::vector<glm::mat4>		GetBrickTransforms();
	std::vector<glm::mat4>		GetBombTransforms();
	std::vector<glm::mat4>		GetBonusTransforms();
	std::vector<MovingEntity*>&	GetEnemies();
	std::vector<Balloon>&		GetBalloons();
	Hero&						GetHero();

	void						plantBomb(glm::vec2 position, int strength);

	void						tickAI(float deltaTime);

private:

	std::vector<Balloon>			mBalloons;
	std::vector<BombAgent>			mBombs;

	std::unique_ptr<Hero>			mHero;
	std::vector<MovingEntity*>		mEnemies;

	std::vector<glm::mat4>		Filter(SquareType type);
	void						recacheEnemies();
};

#endif //BOMBERMAN_Renderer_HPP