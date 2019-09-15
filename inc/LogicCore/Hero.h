#pragma once

#include "Entity/MovingEntity.h"

class Hero : public MovingEntity
{
	int mCurrentBombCount = 1;
	float mLastTimePlacedBomb = 0.f;

public:
	enum PowerupType
	{
		PT_Bombs,
		PT_Flames,
		PT_Speed,
		PT_Wallpass,
		PT_Detonator,
		PT_Bombpass,
		PT_Flamepass,
		PT_NONE
	};

	struct Stats
	{
		int		bombMax = 1;
		int		bombStrength = 1;
		float   movementSpeed;
		uint8_t wallpass = false;
		uint8_t detonator = false;
		uint8_t bombpass = false;
		uint8_t flamepass = false;

		template<class Archive>
		void serialize(Archive & ar)
		{
			ar( bombMax,
				bombStrength,
				movementSpeed,
				wallpass,
				detonator,
				bombpass,
				flamepass);
		}
	};

	Stats mStats;

	Hero(Stats info, glm::vec2 position = {1.5f, 1.5f});
	void increaseBombStrength();
	void increaseBombMax();
	void increaseBombCount();

	void tryPlaceBomb();

	void kill();

	void applyPowerup(PowerupType type);
};
