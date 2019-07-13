#include "Entity/Bomb.h"
#include "Game.hpp"

Bomb::Bomb(glm::vec2 pos, int explosionStrength)
	: Entity(glm::floor(pos) + glm::vec2{0.5f})
	, mExplosionStrength(explosionStrength)
{
	Game::getCollisionInfo()[getPosition()] = SquareType::Bomb;
}

Bomb::~Bomb()
{
	if (auto *hero = &Game::get()->GetHero())
		hero->increaseBombCount();
	Game::getCollisionInfo()[getPosition()] = SquareType::EmptySquare;
}

const float Bomb::FUSE_TIME = 3.0f;
const float Bomb::SPAWN_TIME = .5f;

// SPAWNING
bool Spawning::transition(const Counting&)
{
	return (Game::getCurrentTime() - mStartTime) >= Bomb::SPAWN_TIME;
}

void Spawning::onTick(Bomb& bomb, float DeltaTime)
{
	bomb.setScale((Game::getCurrentTime() - mStartTime) / Bomb::SPAWN_TIME);
}

void Spawning::onEntry(Bomb& bomb)
{
	mStartTime = Game::getCurrentTime();
	bomb.setScale(0.f);
}

void Counting::onTick(Bomb& bomb, float DeltaTime)
{
	if (Game::getCurrentTime() >= mTimeToExplode)
	{
		Game::get()->explosion(bomb.getPosition(), bomb.mExplosionStrength);
		bomb.kill();
	}
}

void Counting::onEntry(Bomb& bomb)
{
	bomb.setScale(1.f);
	mTimeToExplode = Game::getCurrentTime() + Bomb::FUSE_TIME - Bomb::SPAWN_TIME;
}

