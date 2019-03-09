#ifndef BOMBERMAN_Game_HPP
#define BOMBERMAN_Game_HPP
#include <memory>
#include "GameWindow.hpp"
#include "GraphicCore/Renderer.hpp"
#include "InputManagement/InputManager.hpp"
#include "LogicCore/MapForRendering.h"

namespace
{
    int const cDefaultScreenWidth = 640;
    int const cDefaultScreenHeight = 480;
    std::string const cWindowName = "Bomberman";
}

class Game
{
public:
    Game();
    ~Game();
    void start();

private:
	void		resolveCollisions();
    void		doAction(Action const&);
    void		calcDeltaTime();
    void		loadResources();
	void		updateHeroInput();
    std::unique_ptr<GameWindow>      mWindow;
    std::unique_ptr<Renderer>        mRenderer;
    std::unique_ptr<InputManager>    mIManager;

    Uint64                           mTimeNow;
    Uint64                           mTimeLast;
    float                            mDeltaTime;
	MapForRendering					 mMap;
	CollisionInfo					 mCollisionInfo;
    bool                             mIsRunning;

	static float					sInputAcceleration;
};

#endif //BOMBERMAN_Renderer_HPP
