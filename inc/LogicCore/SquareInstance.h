
#ifndef BOMBERMAN_IINSTANCE_H
#define BOMBERMAN_IINSTANCE_H

#include "Entity.h"

enum class SquareType
{
	EmptySquare = 0,
	Wall,
	Brick,
	Bomb,
	Bonus,
	Hero,
	Enemy
};


class SquareInstance : public Entity
{
public:
	SquareInstance(float x, float y, SquareType type);
	~SquareInstance();

	SquareType GetType() const;
	void       ChangeType(SquareType type);

private:
	SquareType mType = SquareType::EmptySquare;
};

#endif
