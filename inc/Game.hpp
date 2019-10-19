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

/*!
\brief The class to hold current game state

It holds info about enemies, hero, map, time, collision.
*/
class Game
{
public:
	/*!
	\brief Gives the instance of Game.
	
	\return The pointer to the already created Game class.
	*/
    static Game             *get();

    Game();
    ~Game();

	/*!
	\brief Starts the game loop.

	This is where the game happends. This function returns only when the game is exiting.
	*/
    void start();
	/*!
	\brief Pauses the game and opens main menu.
	*/
    void pause();
	/*!
	\brief Returns current time in seconds.
	
	\return Current time in seconds. useful for something that keeps track
			of time. For example bombs and menu screens.
	*/
	static float getCurrentTime();
	/*!
	\brief Returns reference to collision info for current map.
	
	\return CollisionInfo struct for queries.
			It only holds blocks, not player or enemies.
	*/
	static CollisionInfo& getCollisionInfo();
	/*!
	\brief Serializing current game state as means of saving game progress.
	*/
	void 		saveCurrentState(std::string fileName  = "ConfigureFile");
	/*!
	\brief Deserializing current game state from file.
	*/
	void 		loadStateFromFile(std::string fileName  = "ConfigureFile");
	/*!
	\brief Propagate window resolution change for whom this is useful.
	*/
	void 		applyWindowChange();
	/*!
	\brief Called on player's winning the stage. Makes a transition to next level.
	*/
    void        stageFinished();
	/*!
	\brief Called on player's death. Plays death animation and cleans-up the level.
	*/
	void        onHeroDied();
	/*!
	\brief Called on player's losing all his lives. You start from scratch.
	*/
	void        gameOver();
	/*!
	\brief Called when someone wants to exit the game or when something went wrong.
	*/
	void        requestExit();
	/*!
	\brief Called from input manager when something important happens.
	*/
    void		doAction(Action);
	/*!
	\brief Utility function for triggering explosion from bombs.

	Handles all the edge-cases (bombs triggering other bombs, walls, bricks).
	\param[in] position Center of the explosion.
	\param[in] span How much ground does this explosion covers.
	*/
    void        explosion(glm::ivec2 position, uint32_t span);

	/*!
	\brief Utility variable.
	*/
    static bool    mReloadStage;
	/*!
	\brief Utility variable.
	*/
    static float   mStageTimer;
	/*!
	\brief Utility variable.
	*/
    static float   mStageStartedTimer;
	/*!
	\brief Is the game running?
	*/
    static bool    mIsRunning;
	/*!
	\brief How much time passed during last frame.
	*/
    static float   mDeltaTime;

	Game(Game const&) = delete;
    Game& operator=(Game const&) = delete;

private:
	void		resolveCollisions();
	void        handleInput();
    void		calcDeltaTime();
    void		loadResources();
    void        loadModels();
	void        cleanupOnStageChange();
    std::shared_ptr<GameWindow>      mWindow;
    std::unique_ptr<Renderer>        mRenderer;
    std::unique_ptr<InputManager>    mIManager;
    std::unique_ptr<KeyboardHandler> mKeyHandler;

    static Uint64                    mTimeNow;
    static Uint64                    mTimeLast;
    static float                     mTimeCorrection;
	static CollisionInfo			 mCollisionInfo;
	static float					 sInputAcceleration;
	static Game						*sInstance;

    bool                             mIsPaused;

	typedef Agent<MovingEntity, BalloonController>	BalloonAgent;
	typedef Agent<MovingEntity, OnilController>		OnilAgent;
	typedef Agent<MovingEntity, OvapeController>	OvapeAgent;
	typedef Agent<Bomb, BombController>				BombAgent;
public:
	/*!
	\brief Called at map creation. Populates map with enemies.
	*/
	void                        addEnemiesOnMap();
	/*!
	\brief Transforms of all walls. Useful for rendering.

	\return std::vector of wall transforms
	*/
	std::vector<glm::mat4>		getWallTransforms();
	/*!
	\brief Transforms of all bricks(destructible walls). Useful for rendering.

	\return std::vector of brick transforms
	*/
	std::vector<glm::mat4>		getBrickTransforms();
	/*!
	\brief Transforms of all bombs. Useful for rendering.

	\return std::vector of bomb transforms
	*/
	std::vector<glm::mat4>		getBombTransforms();
	/*!
	\brief Transform of powerup on map. Useful for rendering.

	\return Matrix transform for this powerup. 
	*/
	glm::mat4					getPowerupTransform();
	/*!
	\brief Transform of exit on map. Useful for rendering.

	\return Matrix transform for the exit. 
	*/
	glm::mat4					getExitTransform();
	/*!
	\brief All of the enemies.

	\return std::vector of pointers to enemies.
	*/
	std::vector<MovingEntity*>&	getEnemies();
	/*!
	\brief Hero reference.

	\return non-const reference to Hero.
	*/
	Hero&						getHero();
	/*!
	\brief Type of powerup on current map.

	\return Hero::PowerupType enum.
	*/
	Hero::PowerupType			powerupTypeOnMap();
	/*!
	\brief Tells whether Hero can proceed to next stage.

	Under the hood counts enemies.
	\return True if player can already exit.
	*/
	bool						isExitActive();
	/*!
	\brief Called after map is desirialized. Extracts info from the map.

	Reading the position of exit and powerup on current map.
	*/
	void						extractInfo();
	/*!
	\brief Creates a new bomb and places it on map.

	\param[in] position Position of new bomb.
	\param[in] strength	Span of the explosion of new bomb.
	*/
	void						plantBomb(glm::vec2 position, int strength);
	/*!
	\brief Updates all AI Controllers.

	\param[in] deltaTime How much time passed during last frame
	*/
	void						tickAI(float deltaTime);

private:
	std::unique_ptr<Hero>			mHero;

	std::vector<BalloonAgent>		mBalloons;
	std::vector<OnilAgent>			mOnils;
	std::vector<OvapeAgent>			mOvapes;
	std::vector<BombAgent>			mBombs;

	std::vector<MovingEntity*>		mEnemies;

	Hero::PowerupType				mPowerupType { Hero::PowerupType::PT_NONE };
	glm::vec2						mPowerup;
	glm::vec2						mExit{ -1, -1 };

	std::vector<glm::mat4>			Filter(SquareType type);
	void							recacheEnemies();

	friend class Renderer;
};

#endif //BOMBERMAN_Renderer_HPP
