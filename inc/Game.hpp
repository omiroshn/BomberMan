#ifndef BOMBERMAN_Game_HPP
#define BOMBERMAN_Game_HPP
#include <memory>
#include "GameWindow.hpp"
#include "GraphicCore/Renderer.hpp"
#include "InputManagement/InputManager.hpp"
#include "LogicCore/MapForRendering.h"
#include "Configure.hpp"

class Game
{
public:
    Game();
    ~Game();
    void start();
    void pause();
	static float getCurrentTime();
	static const CollisionInfo& getCollisionInfo();
	void 		saveCurrentState(std::string fileName  = "ConfigureFile");
	void 		loadStateFromFile(std::string fileName  = "ConfigureFile");
	void 		applyWindowChange();

    static bool    mReloadStage;
    static Uint64  mStageTimer;
    static Uint64  mStageStartedTimer;
    static bool    mIsRunning;

private:
	void		resolveCollisions();
    void		doAction(Action const&);
    void		calcDeltaTime();
    void		loadResources();
	void		updateHeroInput();
    std::shared_ptr<GameWindow>      mWindow;
    std::unique_ptr<Renderer>        mRenderer;
    std::unique_ptr<InputManager>    mIManager;

    static Uint64                    mTimeNow;
    static Uint64                    mTimeLast;
    static float                     mDeltaTime;
	MapForRendering                  mMap;
	static CollisionInfo			 mCollisionInfo;
    bool                             mIsPaused;
	static float					 sInputAcceleration;
};

#endif //BOMBERMAN_Renderer_HPP
