
#ifndef BOMBERMAN_IINSTANCE_H
#define BOMBERMAN_IINSTANCE_H

#include <vector>

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


class SquareInstance
{
public:
	SquareInstance(float x, float y, SquareType type);
	~SquareInstance();
	float GetX() const;
	float GetY() const;
	SquareType GetType() const;
	void SetX(float x);
	void SetY(float y);
	void ChangeType(SquareType type);

private:
	float mX = 0.f;
	float mY = 0.f;
	SquareType mType = SquareType::EmptySquare;
};

#endif
