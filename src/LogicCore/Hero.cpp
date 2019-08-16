#include "LogicCore/Hero.h"
#include "LogicCore/TimerManager.h"
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

Hero::Stats Hero::getStats()
{
	return mStats;
}

void Hero::kill()
{
	AnimateDeath(4);
	mIsDying = true;
	//TimerManager::Instance()->AddTimer(4, false,
		//[] () {
			std::cout << "++++++  kill in Hero, animation set to Dying ++++++" << std::endl;
			Game::get()->onHeroDied();
		//}
	//);
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
		case PowerupType::PT_Speed:
		{
			mStats.movementSpeed += 0.5f;
			break;
		}
		case PowerupType::PT_Wallpass:
		{
			mStats.wallpass = 1;
			break;
		}
		case PowerupType::PT_Detonator:
		{
			mStats.detonator = 1;
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