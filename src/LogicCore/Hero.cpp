#include "LogicCore/Hero.h"
#include "Game.hpp"

Hero::Hero(SaveInfo info, glm::vec2 position /*= {1.5f, 1.5f}*/)
	: MovingEntity(position)
	, mCurrentBombCount(info.bombMax)
	, mBombMax(info.bombMax)
	, mBombStrength(info.bombStrength)
	, mLastTimePlacedBomb(0)
{
}

void Hero::increaseBombStrength()	{ mBombStrength++; }
void Hero::increaseBombMax()		{ mBombMax++; } 
void Hero::increaseBombCount()		{ mCurrentBombCount++; }

void Hero::tryPlaceBomb()
{
	float time = Game::getCurrentTime();
	if (mLastTimePlacedBomb + 1 > time)
		return;

	if (mCurrentBombCount < 1)
		return;
	mCurrentBombCount--;

	mLastTimePlacedBomb = time;
	Game::get()->plantBomb(getPosition(), mBombStrength);
}

Hero::SaveInfo Hero::getSaveInfo()
{
	return SaveInfo(mBombMax, mBombStrength);
}

void Hero::kill()
{
	Game::get()->onHeroDied();
}