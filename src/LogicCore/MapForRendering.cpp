#include "Game.hpp"
#include <imgui.h>
#include <algorithm>
#include "Entity/MovingEntity.h"

std::vector<glm::mat4> Game::Filter(SquareType type)
{
	std::vector<glm::mat4> Result;
	auto& squares = mCollisionInfo.Squares;
	for (int i = 0; i < squares.size(); i++)
	{
		glm::vec2 Position {i % mCollisionInfo.width, i / mCollisionInfo.width};
		auto value = squares[i];
		if (value == type)
			Result.push_back(glm::translate(glm::mat4(1), glm::vec3{ Position.x + 0.5, 0, Position.y + 0.5}));
	}
	return Result;
}

void Game::recacheEnemies()
{
	mEnemies.clear();
	mBalloons.erase(std::remove_if(mBalloons.begin(), mBalloons.end(), [](const MovingEntity *balloon){
		return balloon->isDead();
	}), mBalloons.end());

	for (auto& It : mBalloons)
		mEnemies.push_back(It);
}

std::vector<glm::mat4> Game::GetWallTransforms() {
	return Filter(SquareType::Wall);
}

std::vector<glm::mat4> Game::GetBrickTransforms() {
	return Filter(SquareType::Brick);
}

std::vector<glm::mat4> Game::GetBombTransforms() {
	return Filter(SquareType::Bomb);
}

std::vector<glm::mat4> Game::GetBonusTransforms() {
	return Filter(SquareType::Bonus);
}

MovingEntity& Game::GetHero()
{
	return *mHero;
}

std::vector<MovingEntity*>& Game::GetEnemies()
{
	return mEnemies;
}

std::vector<Game::Balloon>& Game::GetBalloons()
{
	return mBalloons;
}