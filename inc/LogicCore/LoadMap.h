#ifndef BOMBERMAN_LOADMAP_H
#define BOMBERMAN_LOADMAP_H

#include "LogicCore/MapGeneration.hpp"
#include "LogicCore/SquareInstance.h"
#include "LogicCore/Serialization.hpp"

struct CollisionInfo {
	std::vector<SquareType> Squares;
    unsigned				width{0};
	SquareType& operator[](glm::ivec2);
	SquareType& operator[](glm::vec2);
};

class MapLoader
{
public:
	MapLoader();
	~MapLoader();

	MapLoader(MapLoader const&) = delete;
    MapLoader& operator=(MapLoader const&) = delete;

	CollisionInfo GetMap(const int index);
	void UpdateMap();
	bool MapIsLoaded();
	void cleanMapForRendering();

private:
	std::vector<SquareType>      mMapOfDigits;
    bool                         mLoaded{false};
    unsigned                     mWidth;
	Serialization                mReaderWriter;
};

#endif