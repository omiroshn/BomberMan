
#include "LogicCore/LoadMap.h"
#include <iostream>

MapLoader::MapLoader() {

}

MapLoader::~MapLoader() {

}

std::vector<SquareInstance*> MapLoader::GetMap(const int index)
{
	if (index == -1)
	{
		MapGenerator mapGenerator(0);
		mMapOfDigits = mapGenerator.GenerateNewMap();
		mWidth = mapGenerator.GetWidth();
		mHeight = mapGenerator.GetHeight();
		ConvertDigitsToInstances();
		mLoaded = true;
	}
	else if (index >= 0)
	{
		//load from disk
		ConvertDigitsToInstances();
		mLoaded = true;
	}
	return mMapOfInstances;

}

void MapLoader::UpdateMap()
{

}

bool MapLoader::MapIsLoaded()
{
	return mLoaded;
}

void MapLoader::ConvertDigitsToInstances()
{
	int size = mMapOfDigits.size();
	mMapOfInstances.reserve(size);

	for (int i = 0; i < size; ++i)
	{
		int x = i / (mWidth);
		int y =  (i % mWidth);

		std::cout << "x = " << x << "     y = " << y << std::endl;
		if (mMapOfDigits.at(i) == 1)
		{
			mMapOfInstances.push_back(new SquareInstance((float)x, (float)y, SquareType::Wall));
		}
		else if (mMapOfDigits.at(i) == 2)
		{
			mMapOfInstances.push_back(new SquareInstance((float)x, (float)y, SquareType::Brick));
		}

	}
}
