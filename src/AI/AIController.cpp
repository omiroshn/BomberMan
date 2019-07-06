#include "AI/AIController.h"
#include "Game.hpp"

/** AI utility functions - start */
namespace
{
	enum class EDirection : uint8_t {
		NONE	= 0,
		DOWN	= 1,
		RIGHT	= 1 << 1,
		UP		= 1 << 2,
		LEFT	= 1 << 3
	};

	glm::vec2 directionToVector(EDirection dir)
	{
		switch (dir)
		{
		case EDirection::DOWN:
			return glm::vec2(0,1);
		case EDirection::RIGHT:
			return glm::vec2(1,0);
		case EDirection::UP:
			return glm::vec2(0,-1);
		case EDirection::LEFT:
			return glm::vec2(-1,0);
		case EDirection::NONE:
		default:
			return glm::vec2(0);
		}
	}

	inline bool isEmpty(glm::vec2 position)
	{
		auto& collision = Game::getCollisionInfo();
		return collision[position] == SquareType::EmptySquare;
	}

	EDirection getAvailableDirections(Entity& entity) {
		union {
			uint8_t mask;
			EDirection result;
		};
		result = EDirection::NONE;
		if (isEmpty(entity.getPosition() + glm::vec2(0,1)))
			mask |= (uint8_t)EDirection::DOWN;
		if (isEmpty(entity.getPosition() + glm::vec2(1,0)))
			mask |= (uint8_t)EDirection::RIGHT;
		if (isEmpty(entity.getPosition() + glm::vec2(0,-1)))
			mask |= (uint8_t)EDirection::UP;
		if (isEmpty(entity.getPosition() + glm::vec2(-1,0)))
			mask |= (uint8_t)EDirection::LEFT;
		return result;
	}

	EDirection randomDir() {
		union {
			uint8_t mask;
			EDirection result;
		};
		mask = 1 << (std::rand() & 3);
		return result;
	}

	glm::vec2 pickRandomDirection(Entity& pawn)
	{
		union {
			uint8_t mask;
			EDirection result;
		};
		result = getAvailableDirections(pawn);
		if (result == EDirection::NONE)
			return glm::vec2(0);
		uint8_t i = 1 << (std::rand() & 3);
		while (true)
		{
			if (i > 8)
				i = 1;
			if ((uint8_t)result & i)
				return directionToVector((EDirection)((uint8_t)result & i));
			i <<= 1;
		}
		return glm::vec2(0,1);
	}

	void	recenter(MovingEntity& pawn)
	{
		glm::vec2 position = pawn.getPosition();
		glm::vec2 closestCenter = (glm::floor(position) + glm::ceil(position)) / 2.f;
		pawn.AddVelocity(closestCenter - position);
	}

	bool	moveAI(MovingEntity& pawn, glm::vec2 destination, float deltaTime)
	{
		auto& info = Game::getCollisionInfo();
		glm::vec2 position = pawn.getPosition();
		glm::vec2 direction = destination - position;
		float length = glm::length(direction);

		if (length < 0.02)
		{
			recenter(pawn);
			return false;
		}

		glm::vec2 acceleration = direction / length * 40.f;

		pawn.AddAcceleration(acceleration);
		return true;
	}

	glm::vec2	march(glm::vec2 start, glm::vec2 direction, int distance)
	{
		auto& info = Game::getCollisionInfo();
		
		for (int i = 1; i <= distance; i++)
		{
			if (info[start + direction] == SquareType::EmptySquare)
				start += direction;
		}
		return (glm::floor(start) + glm::ceil(start)) / 2.f;
	}

	bool checkVisibility(MovingEntity& a, MovingEntity& b)
	{
		auto& info = Game::getCollisionInfo();

		glm::ivec2 positionA = a.getPosition();
		glm::ivec2 positionB = b.getPosition();

		if (positionA == positionB)
			return true;

		if (positionA.x == positionB.x)
		{
			int min = glm::min(positionA.y, positionB.y);
			int max = glm::max(positionA.y, positionB.y);

			for (int i = min; i < max; i++)
				if (info[glm::ivec2{positionA.x, i}] != SquareType::EmptySquare)
					return false;

			return true;
		}
		else if (positionA.y == positionB.y)
		{
			int min = glm::min(positionA.x, positionB.x);
			int max = glm::max(positionA.x, positionB.x);

			for (int i = min; i < max; i++)
				if (info[glm::ivec2{i, positionA.x}] != SquareType::EmptySquare)
					return false;

			return true;
		}
		return false;
	}
}
/** AI utility functions - end */

/* Idle state -  start */
void IdleState::onEntry(MovingEntity&)		   { m_TransitionToPatrol = Game::getCurrentTime() + .3f; }
bool IdleState::transition(const PatrolState&) { return m_TransitionToPatrol <= Game::getCurrentTime(); }
/* Idle state -  end */

/* Patrol state - start */
void PatrolState::onTick(MovingEntity& pawn, float deltaTime)
{
	const glm::bvec2 NotZeroVec = glm::notEqual(mCurrentDirection, glm::vec2(0));
	const bool HasDirection = NotZeroVec.x || NotZeroVec.y;

	if (!moveAI(pawn, mShortTermGoal, deltaTime))
	{
		mShouldIdle = true;
		return;
	}

	MapForRendering *map = Game::getMap();
	auto& Hero = map->GetHero();
	mPawnSeesPlayer = checkVisibility(pawn, Hero);
}
bool PatrolState::transition(const IdleState&)
{
	return mShouldIdle;
}

bool PatrolState::transition(const ChaseState&)
{
	return mPawnSeesPlayer;
}

void PatrolState::onEntry(MovingEntity& Pawn, float DeltaTime /*= 0*/)
{
	auto& info = Game::getCollisionInfo();

	mCurrentDirection = pickRandomDirection(Pawn);
	glm::vec2 start = Pawn.getPosition();
	int distance = rand() % info.width;
	mShortTermGoal = march(start, mCurrentDirection, distance);
	mShouldIdle = false;
}
/* Patrol state -  end */


/* Chase state - start */
void ChaseState::onTick(MovingEntity& pawn, float deltaTime)
{
	MapForRendering *map = Game::getMap();
	auto& Hero = map->GetHero();
	mPawnSeesPlayer = checkVisibility(pawn, Hero);
	if (mPawnSeesPlayer)
		mLastSeenPlayer = (glm::floor(Hero.getPosition()) + glm::ceil(Hero.getPosition())) / 2.f;
	if (!moveAI(pawn, mLastSeenPlayer, deltaTime))
	{
		mIsConfused = true;
		return;
	}
}

bool ChaseState::transition(const IdleState&)
{
	return mIsConfused;
}

bool ChaseState::transition(const ConfusedState&)
{
	return mPawnSeesPlayer;
}

void ChaseState::onEntry(MovingEntity& Pawn, float DeltaTime /*= 0*/)
{
	mPawnSeesPlayer = true;
	mIsConfused = false;
}
/* Chase state -  end */