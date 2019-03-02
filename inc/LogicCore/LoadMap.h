#ifndef BOMBERMAN_LOADMAP_H
#define BOMBERMAN_LOADMAP_H

#include "LogicCore/MapGeneration.hpp"

class MapLoader
{
public:
	MapLoader();
	~MapLoader();
	std::vector<SquareInstance*>& GetMap(const int index);
	void UpdateMap();
	bool MapIsLoaded();

private:
	std::vector<SquareInstance*> mMapOfInstances;
	std::vector<int>	         mMapOfDigits;
	bool                         mLoaded = false;
	int                          mWidth = 30;
	int                          mHeight = 20;

private:
	void ConvertDigitsToInstances();
};

#endif
