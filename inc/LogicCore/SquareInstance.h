
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
	Powerup_Speed		= 6,
	Powerup_Wallpass	= 7,
	Powerup_Detonator	= 8,
	Powerup_Bombpass	= 9,
	Powerup_Flamepass 	= 10,
	Powerup_Mystery		= 11,
	Exit				= 12
};

#endif
