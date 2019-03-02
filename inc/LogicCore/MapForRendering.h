
#ifndef BOMBERMAN_MAPFORRENDERING_H
#define BOMBERMAN_MAPFORRENDERING_H

#include <LogicCore/SquareInstance.h>
#include <vector>

class MapForRendering
{
public:
	MapForRendering(std::vector<SquareInstance*> map);
	~MapForRendering();
	std::vector<SquareInstance*> GetEmptySquares();
	std::vector<SquareInstance*> GetWalls();
	std::vector<SquareInstance*> GetBricks();
	std::vector<SquareInstance*> GetBombs();
	std::vector<SquareInstance*> GetBonuses();
	std::vector<SquareInstance*> GetHeroes();
	std::vector<SquareInstance*> GetEnemies();

	void ParseMapBySquareInstances();

private:
	std::vector<SquareInstance*> mRawMap;
	std::vector<SquareInstance*> mEmptySquares;
	std::vector<SquareInstance*> mWalls;
	std::vector<SquareInstance*> mBricks;
	std::vector<SquareInstance*> mBombs;
	std::vector<SquareInstance*> mBonuses;
	std::vector<SquareInstance*> mHeroes;
	std::vector<SquareInstance*> mEnemies;

};

#endif
