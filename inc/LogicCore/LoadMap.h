#ifndef BOMBERMAN_LOADMAP_H
#define BOMBERMAN_LOADMAP_H

#include "LogicCore/MapGeneration.hpp"

struct CollisionInfo {
	std::vector<SquareType> Squares;
	unsigned				width;
	SquareType operator[](glm::ivec2) const;
	SquareType operator[](glm::vec2) const;
};

class MapLoader
{
public:
	MapLoader();
	~MapLoader();
	std::tuple<std::vector<SquareInstance*>, CollisionInfo> GetMap(const int index);
	void UpdateMap();
	bool MapIsLoaded();

private:
	std::vector<SquareInstance*> mMapOfInstances;
	std::vector<SquareType>      mMapOfDigits;
	bool                         mLoaded = false;
	unsigned                     mWidth;
	unsigned                     mHeight;

private:
	void ConvertDigitsToInstances();
};

#endif
