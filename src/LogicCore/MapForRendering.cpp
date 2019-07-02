#include "LogicCore/MapForRendering.h"


MapForRendering::MapForRendering(const std::vector<SquareInstance*>& map) :
	mRawMap(map),
	mHero(new MovingEntity(glm::vec2(1.5,1.5)))
{

}

MapForRendering::MapForRendering()
{

}

MapForRendering::~MapForRendering() {

}

std::vector<SquareInstance *> MapForRendering::Filter(SquareType type)
{
	std::vector<SquareInstance *> Result;
	for (auto It : mRawMap)
		if (It->GetType() == type)
			Result.push_back(It);
	return Result;
}

void MapForRendering::recacheEnemies()
{
	mEnemies.clear();
	for (auto& It : mBalloons)
		mEnemies.push_back(It);
}

std::vector<SquareInstance *> MapForRendering::GetWalls() {
	return Filter(SquareType::Wall);
}

std::vector<SquareInstance *> MapForRendering::GetBricks() {
	return Filter(SquareType::Brick);
}

std::vector<SquareInstance *> MapForRendering::GetBombs() {
	return Filter(SquareType::Bomb);
}

std::vector<SquareInstance *> MapForRendering::GetBonuses() {
	return Filter(SquareType::Bonus);
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
	return true;
}

MovingEntity& MapForRendering::GetHero()
{
	return *mHero;
}

std::vector<MovingEntity*>& MapForRendering::GetEnemies()
{
	recacheEnemies();
	return mEnemies;
}

std::vector<SquareInstance *> &MapForRendering::GetRawMap()
{
	return mRawMap;
}
