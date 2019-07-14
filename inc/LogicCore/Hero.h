#pragma once

#include "Entity/MovingEntity.h"


class Hero : public MovingEntity
{

	int mBombMax = 1;
	int mCurrentBombCount = 1;
	int mBombStrength = 1;

	float mLastTimePlacedBomb;
public:
	struct SaveInfo
	{
		SaveInfo(int i_bombMax, int i_bombStrength)
		: bombMax(i_bombMax)
		, bombStrength(i_bombStrength)
		{}
		
		int bombMax = 20;
		int bombStrength = 20;
	};

	Hero(SaveInfo info, glm::vec2 position = {1.5f, 1.5f});
	void increaseBombStrength();
	void increaseBombMax();

	void increaseBombCount();

	void tryPlaceBomb();

	SaveInfo getSaveInfo();
	void kill();
};