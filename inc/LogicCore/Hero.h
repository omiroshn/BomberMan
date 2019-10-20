#pragma once

#include "Entity/MovingEntity.h"
/*!
\brief The class that represents Hero

Holds everything a hero needs and what's not included in MovingEntity.
It includes info about what kind of bombs he places and what kind of powerups he picked up.
*/
class Hero : public MovingEntity
{
	int mCurrentBombCount = 1;
	float mLastTimePlacedBomb = 0.f;

public:
/*!
\brief Types of powerup Hero can pick up
*/
	enum PowerupType
	{
		PT_Bombs,
		PT_Flames,
		PT_Wallpass,
		PT_Bombpass,
		PT_Flamepass,
		PT_NONE
	};

/*!
\brief Stats of Hero. It changes when he picks up powerups.
*/
	struct Stats
	{
		int		bombMax = 1;
		int		bombStrength = 1;
		uint8_t wallpass = false;
		uint8_t detonator = false;
		uint8_t bombpass = false;
		uint8_t flamepass = false;

		template<class Archive>
		void serialize(Archive & ar)
		{
			ar( bombMax,
				bombStrength,
				wallpass,
				detonator,
				bombpass,
				flamepass);
		}
	};

	Stats mStats;

	Hero(Stats info, glm::vec2 position = {1.5f, 1.5f});
/*!
\brief After call to this function Hero will place more powerful bombs.
*/
	void increaseBombStrength();
/*!
\brief After call to this function Hero will be able to place more bombs.
*/
	void increaseBombMax();
/*!
\brief This is isially called by a bomb when it's exploding.

This will give back one bomb to the player.
*/
	void increaseBombCount();
/*!
\brief Makes necessary checks that player really can place a
bomb at current spot and then places it.
*/
	void tryPlaceBomb();
/*!
\brief Makes everything ready to play death animation.
*/
	void kill();
/*!
\brief Applies powerup effects to player.
*/
	void applyPowerup(PowerupType type);
};
