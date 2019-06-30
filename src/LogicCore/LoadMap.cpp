
#include "LogicCore/LoadMap.h"
#include <tuple>
#include <iostream>

MapLoader::MapLoader()
{

}

MapLoader::~MapLoader()
{

}

SquareType CollisionInfo::operator[](glm::ivec2 coords) const
{
	const unsigned index = coords.x + coords.y * width;
	if (coords.x < 0 || coords.y < 0 || coords.x >= (int)width || index >= Squares.size())
		return SquareType::Wall;
	return Squares[index];
}

SquareType CollisionInfo::operator[](glm::vec2 coords) const
{
	return operator[](glm::ivec2(coords.x, coords.y));
}

std::tuple<std::vector<SquareInstance*>, CollisionInfo> MapLoader::GetMap(const int index)
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
		mMapOfDigits = mReaderWriter.LoadCampaignMap(index);
		mWidth = mMapOfDigits.size() / 20;
		mHeight = mMapOfDigits.size() / mWidth;
		ConvertDigitsToInstances();
		mLoaded = true;
	}
	return std::make_tuple(mMapOfInstances, CollisionInfo{ mMapOfDigits, mWidth });
}

void MapLoader::UpdateMap()
{
	mLoaded = false;
}

bool MapLoader::MapIsLoaded()
{
	return mLoaded;
}

void MapLoader::ConvertDigitsToInstances()
{
	unsigned size = mMapOfDigits.size();
	mMapOfInstances.reserve(size);

	for (unsigned i = 0; i < size; ++i)
	{

		unsigned y = i / mWidth;
		unsigned x = i % mWidth;

		//std::cout << "x = " << x << "     y = " << y << std::endl;
		if ((unsigned)mMapOfDigits.at(i) == 1)
		{
			mMapOfInstances.push_back(new SquareInstance(static_cast<float>(x), static_cast<float>(y), SquareType::Wall));
		}
		else if ((unsigned)mMapOfDigits.at(i) == 2)
		{
			mMapOfInstances.push_back(new SquareInstance(static_cast<float>(x), static_cast<float>(y), SquareType::Brick));
		}

		mMapOfInstances.push_back(new SquareInstance(static_cast<float>(x), static_cast<float>(y), mMapOfDigits[i]));
	}
}
