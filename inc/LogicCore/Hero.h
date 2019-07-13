#pragma once

#include "Entity/MovingEntity.h"


class Hero : public MovingEntity
{
	int mCurrentBombCount;

	int mBombMax;
	int mBombStrength;

	float mLastTimePlacedBomb;
public:
	struct SaveInfo
	{
		int bombMax = 1;
		int bombStrength = 1;
	};

	Hero(SaveInfo info, glm::vec2 position = {1.5f, 1.5f});
	void increaseBombStrength();
	void increaseBombMax();

	void increaseBombCount();

	void tryPlaceBomb();
};