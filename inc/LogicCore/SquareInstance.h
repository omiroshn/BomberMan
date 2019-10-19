
#ifndef BOMBERMAN_IINSTANCE_H
#define BOMBERMAN_IINSTANCE_H

#include "Entity/Entity.h"

enum class SquareType : uint_fast8_t
{
	EmptySquare 		= 0,
	Wall				= 1,
	Brick				= 2,
	Bomb				= 3,
	Powerup_Bombs		= 4,
	Powerup_Flames		= 5,
	Powerup_Wallpass	= 6,
	Powerup_Detonator	= 7,
	Powerup_Bombpass	= 8,
	Powerup_Flamepass 	= 9,
	Powerup_Mystery		= 10,
	Exit				= 11
};

#endif
