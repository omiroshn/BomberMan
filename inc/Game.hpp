#ifndef BOMBERMAN_Game_HPP
#define BOMBERMAN_Game_HPP
#include <memory>
#include "GameWindow.hpp"
#include "GraphicCore/Renderer.hpp"
#include "InputManagement/InputManager.hpp"
#include "InputManagement/KeyboardHandler.hpp"
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
	static CollisionInfo& getCollisionInfo();
	void 		saveCurrentState(std::string fileName  = "ConfigureFile");
	void 		loadStateFromFile(std::string fileName  = "ConfigureFile");
	void 		applyWindowChange();
    void        stageFinished();

    static bool    mReloadStage;
    static float   mStageTimer;
    static float   mStageStartedTimer;
    static bool    mIsRunning;

    void        explosion(glm::ivec2 position, uint32_t span);

    static Game             *get();
    static MapForRendering  *getMap();
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
	MapForRendering                  mMap;
	static CollisionInfo			 mCollisionInfo;
    bool                             mIsPaused;
	static float					 sInputAcceleration;
	static Game					    *sInstance;
};

#endif //BOMBERMAN_Renderer_HPP