#pragma once

#include "Entity/MovingEntity.h"


class Hero : public MovingEntity
{
	int mCurrentBombCount = 1;

	int mBombMax = 1;
	int mBombStrength = 1;

	float mLastTimePlacedBomb;
public:
	struct SaveInfo
	{
		SaveInfo(int i_bombMax, int i_bombStrength)
		: bombMax(i_bombMax)
		, bombStrength(i_bombStrength)
		{}
		
		int bombMax = 1;
		int bombStrength = 1;
	};

	Hero(SaveInfo info, glm::vec2 position = {1.5f, 1.5f});
	void increaseBombStrength();
	void increaseBombMax();

	void increaseBombCount();

	void tryPlaceBomb();

	SaveInfo getSaveInfo();
	void kill();
};