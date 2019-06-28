
#ifndef BOMBERMAN_MAPFORRENDERING_H
#define BOMBERMAN_MAPFORRENDERING_H

#include <LogicCore/SquareInstance.h>
#include <vector>
#include "Entity/MovingEntity.h"
#include "AI/AIController.h"

class MapForRendering
{
public:
	MapForRendering();
	MapForRendering(const std::vector<SquareInstance*>& map);
	~MapForRendering();
	std::vector<SquareInstance*>	GetEmptySquares();
	std::vector<SquareInstance*>	GetWalls();
	std::vector<SquareInstance*>	GetBricks();
	std::vector<SquareInstance*>	GetBombs();
	std::vector<SquareInstance*>	GetBonuses();

	bool     cleanMapForRendering();
	std::vector<BalloonController>& GetControllers();
	std::vector<SquareInstance*>&	GetRawMap();
	std::vector<MovingEntity*>&		GetEnemies();
	MovingEntity& GetHero();

	unsigned getWitdh();
	unsigned getHeight();
private:
	unsigned						mWidth;
	unsigned						mHeight;
	std::vector<SquareInstance*>	mRawMap;

	// std::vector<SquareInstance*>	mEmptySquares;
	// std::vector<SquareInstance*>	mWalls;
	// std::vector<SquareInstance*>	mBricks;
	// std::vector<SquareInstance*>	mBombs;
	// std::vector<SquareInstance*>	mBonuses;

	// std::vector<SquareType>			mCollisionMap;
	std::vector<MovingEntity*>		mEnemies;
	std::vector<BalloonController>	mControllers;
	MovingEntity					*mHero;

	std::vector<SquareInstance*>	Filter(SquareType type);
};

#endif
