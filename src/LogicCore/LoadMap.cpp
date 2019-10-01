#include "LogicCore/LoadMap.h"
#include <tuple>
#include <iostream>

MapLoader::MapLoader() : mWidth{0}
{

}

MapLoader::~MapLoader()
{

}

SquareType& CollisionInfo::operator[](glm::ivec2 coords)
{
	const unsigned index = coords.x + coords.y * width;
	static SquareType WallType;
	WallType = SquareType::Wall;
	if (coords.x < 0 || coords.y < 0 || coords.x >= (int)width || index >= Squares.size())
		return WallType;
	return Squares[index];
}

SquareType& CollisionInfo::operator[](glm::vec2 coords)
{
	return operator[](glm::ivec2(coords.x, coords.y));
}

CollisionInfo MapLoader::GetMap(const int index)
{
	if (index > 0)
	{
		//load from disk
		mMapOfDigits = mReaderWriter.LoadCampaignMap(index, mWidth);
		mLoaded = true;
	}
	else
	{
		MapGenerator mapGenerator(0);
		mMapOfDigits = mapGenerator.GenerateNewMap();
		mWidth = mapGenerator.GetWidth();
		mLoaded = true;
	}
	return CollisionInfo{ mMapOfDigits, mWidth };
}

void MapLoader::cleanMapForRendering()
{
	mMapOfDigits.clear();
	mLoaded = false;
}

void MapLoader::UpdateMap()
{
	mLoaded = false;
}

bool MapLoader::MapIsLoaded()
{
	return mLoaded;
}

