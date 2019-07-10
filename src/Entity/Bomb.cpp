#include "Entity/Bomb.h"
#include "Game.hpp"
//#include <glm/glm.hpp>

const float Bomb::FUSE_TIME = 2.0f;
const float Bomb::SPAWN_TIME = 0.5f;

// SPAWNING
bool Spawning::transition(const Counting&)
{
	return (Game::getCurrentTime() - mStartTime) >= Bomb::SPAWN_TIME;
}

void Spawning::onTick(Bomb& bomb, float DeltaTime)
{
	bomb.setScale((Game::getCurrentTime() - mStartTime) * 2);
}

void Spawning::onEntry(Bomb& bomb)
{
	mStartTime = Game::getCurrentTime();
	bomb.setScale(0.f);
}

// COUNTING
bool Counting::transition(const Exploding&)
{
	return mTimeToExplode >= Game::getCurrentTime();
}

void Counting::onTick(Bomb& bomb, float DeltaTime)
{
	bomb.setScale(1.f + glm::sin(mTimeToExplode - Game::getCurrentTime()));
}

void Counting::onEntry(Bomb& bomb)
{
	bomb.setScale(1.f);
	mTimeToExplode = Game::getCurrentTime() + Bomb::FUSE_TIME - 0.5f;
}

// EXPLODING
void Exploding::onTick(Bomb& bomb, float DeltaTime)
{
	bomb.setScale(glm::mix(bomb.getScale(), 0.f, DeltaTime));
	if ((Game::getCurrentTime() - mExplosionTime) > 0.5f)
		bomb.kill();
}

void Exploding::onEntry(Bomb& bomb)
{
	mExplosionTime = Game::getCurrentTime();
	Game::get()->explosion(bomb.getPosition(), 2);
}

