
#ifndef BOMBERMAN_IINSTANCE_H
#define BOMBERMAN_IINSTANCE_H

#include "Entity/Entity.h"

enum class SquareType : uint_fast8_t
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
