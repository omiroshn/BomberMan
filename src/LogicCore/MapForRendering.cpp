#include "LogicCore/MapForRendering.h"

MapForRendering::MapForRendering(const std::vector<SquareInstance*>& map) :
	mRawMap(map),
	mHero(new MovingEntity(glm::vec2(1,1)))
{

}

MapForRendering::MapForRendering()
{

}

MapForRendering::~MapForRendering() {

}

std::vector<SquareInstance *> MapForRendering::GetEmptySquares() {
	return mEmptySquares;
}

std::vector<SquareInstance *> MapForRendering::GetWalls() {
	return mWalls;
}

std::vector<SquareInstance *> MapForRendering::GetBricks() {
	return mBricks;
}

std::vector<SquareInstance *> MapForRendering::GetBombs() {
	return mBombs;
}

std::vector<SquareInstance *> MapForRendering::GetBonuses() {
	return mBonuses;
}

unsigned MapForRendering::getWitdh()
{
	return mWidth;
}

unsigned MapForRendering::getHeight()
{
	return mHeight;
}

bool   MapForRendering::cleanMapForRendering()
{
	mRawMap.clear();
	mEmptySquares.clear();
	mWalls.clear();
	mBricks.clear();
	mBombs.clear();
	mBonuses.clear();
	mCollisionMap.clear();
	mEnemies.clear();
	return true;
}

MovingEntity& MapForRendering::GetHero()
{
	return *mHero;
}

std::vector<MovingEntity*>& MapForRendering::GetEnemies()
{
	return mEnemies;
}

std::vector<BalloonController>& MapForRendering::GetControllers()
{
	return mControllers;
}

void MapForRendering::ParseMapBySquareInstances() {
	for (unsigned int i = 0; i < mRawMap.size(); ++i)
	{
		if (mRawMap.at(i)->GetType() == SquareType::Wall)
			mWalls.push_back(mRawMap.at(i));
		else if (mRawMap.at(i)->GetType() == SquareType::Brick)
			mBricks.push_back(mRawMap.at(i));
	}
}
