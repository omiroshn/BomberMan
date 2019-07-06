
#ifndef BOMBERMAN_MAPFORRENDERING_H
#define BOMBERMAN_MAPFORRENDERING_H

#include <LogicCore/SquareInstance.h>
#include <vector>
#include "Entity/MovingEntity.h"
#include "AI/AIController.h"
#include "AI/Agent.h"

class MapForRendering
{
	typedef Agent<MovingEntity, BalloonController> Balloon;
public:
	MapForRendering();
	MapForRendering(const std::vector<SquareInstance*>& map);
	~MapForRendering();
	std::vector<SquareInstance*>	GetWalls();
	std::vector<SquareInstance*>	GetBricks();
	std::vector<SquareInstance*>	GetBombs();
	std::vector<SquareInstance*>	GetBonuses();

	bool     cleanMapForRendering();
	std::vector<SquareInstance*>&	GetRawMap();
	std::vector<MovingEntity*>&		GetEnemies();
	std::vector<Balloon>&			GetBalloons();
	MovingEntity&					GetHero();
	void							tick(float deltaTime);

	unsigned getWitdh();
	unsigned getHeight();
private:

	unsigned						mWidth;
	unsigned						mHeight;
	std::vector<SquareInstance*>	mRawMap;
	std::vector<Balloon>			mBalloons;

	MovingEntity					*mHero;
	std::vector<MovingEntity*>		mEnemies;

	std::vector<SquareInstance*>	Filter(SquareType type);
	void recacheEnemies();
};

#endif
