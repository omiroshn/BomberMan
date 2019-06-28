#ifndef BOMBERMAN_Game_HPP
#define BOMBERMAN_Game_HPP
#include <memory>
#include "GameWindow.hpp"
#include "GraphicCore/Renderer.hpp"
#include "InputManagement/InputManager.hpp"
#include "LogicCore/MapForRendering.h"

class Game
{
public:
    Game();
    ~Game();
    void start();
    void pause();
	static float getCurrentTime();
	static const CollisionInfo& getCollisionInfo();
    static bool  mIsRunning;
    static int   mMusicVolume;
    static int   mSoundsVolume;
    static int   mKeyBindVolume;
    static int   mScreenResolution;
    static bool  mWindowed;

private:
	void		resolveCollisions();
    void		doAction(Action const&);
    void		calcDeltaTime();
    void		loadResources();
	void		updateHeroInput();
    std::unique_ptr<GameWindow>      mWindow;
    std::unique_ptr<Renderer>        mRenderer;
    std::unique_ptr<InputManager>    mIManager;

    static Uint64                    mTimeNow;
    static Uint64                    mTimeLast;
    static float                     mDeltaTime;
	MapForRendering                  mMap;
	static CollisionInfo			 mCollisionInfo;
    bool                             mIsPaused;
	static float					sInputAcceleration;
};

#endif //BOMBERMAN_Renderer_HPP
