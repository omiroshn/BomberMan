#include "LogicCore/Hero.h"
#include "ResourceManagement/MusicPlayer.hpp"
#include "Game.hpp"

Hero::Hero(Stats info, glm::vec2 position /*= {1.5f, 1.5f}*/)
	: MovingEntity(position)
	, mCurrentBombCount(info.bombMax)
	, mStats(info)
{
}

void Hero::increaseBombStrength()	{ mStats.bombStrength++; }
void Hero::increaseBombMax()		{ mStats.bombMax++; } 
void Hero::increaseBombCount()		{ mCurrentBombCount++; }

void Hero::tryPlaceBomb()
{
	float time = Game::getCurrentTime();

	if (mLastTimePlacedBomb + 0.5f > time
	|| Game::getCollisionInfo()[getPosition()] == SquareType::Bomb
	|| mCurrentBombCount < 1)
		return;

	mCurrentBombCount--;
	mLastTimePlacedBomb = time;
	Game::get()->plantBomb(getPosition(), mStats.bombStrength);
}

void Hero::kill()
{
	if (mDead)
		return;
	MUSIC_PLAYER.playSound("death");
	getAnimation().setTime(0);
	MovingEntity::kill();
}

void Hero::applyPowerup(PowerupType type)
{
	switch (type)
	{
		case PowerupType::PT_Bombs:
		{
			increaseBombMax();
			increaseBombCount();
			break;
		}
		case PowerupType::PT_Flames:
		{
			increaseBombStrength();
			break;
		}
		case PowerupType::PT_Wallpass:
		{
			mStats.wallpass = 1;
			break;
		}
		case PowerupType::PT_Bombpass:
		{
			mStats.bombpass = 1;
			break;
		}
		case PowerupType::PT_Flamepass:
		{
			mStats.flamepass = 1;
			break;
		}
		default: break;
	}
}