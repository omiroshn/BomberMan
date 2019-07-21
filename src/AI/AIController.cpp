#include "AI/AIController.h"
#include "Game.hpp"
#include "Entity/MovingEntity.h"
#include "glm/common.hpp"

/** AI utility functions - start */
namespace
{
    #undef min
    #undef max
	enum class EDirection : uint8_t {
		NONE	= 0,
		DOWN	= 1,
		RIGHT	= 1 << 1,
		UP		= 1 << 2,
		LEFT	= 1 << 3
	};

	glm::vec2 closestCenter(glm::vec2 position)
	{
		return glm::floor(position) + glm::vec2(0.5f);
	}

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
		glm::vec2 center = closestCenter(position);
		pawn.AddVelocity(center - position);
	}

	bool	moveAI(MovingEntity& pawn, glm::vec2 destination)
	{
		glm::vec2 position = pawn.getPosition();

		glm::vec2 offRails = closestCenter(position) - position;
		glm::vec2 absOffset = glm::abs(offRails);
		if (absOffset.x > 0.02f && absOffset.y > 0.02f)
		{
			glm::vec2 correctionDirection;
			if (absOffset.x < absOffset.y)
				correctionDirection = glm::vec2{ offRails.x, 0 };
			else
				correctionDirection = glm::vec2{ 0, offRails.y };
			pawn.AddAcceleration(glm::normalize(correctionDirection) * 10.f);
		}
		
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
		
		for (int i = 0; i < distance; i++)
		{
			if (info[start + direction] != SquareType::EmptySquare)
				break;
			start += direction;
		}
		return closestCenter(start);
	}

	bool checkVisibility(MovingEntity& pawn, glm::vec2 point)
	{
		glm::vec2 direction = glm::normalize(point - pawn.getPosition());
		float angle = pawn.getAngle();
		glm::vec2 visionDirection {glm::sin(angle), glm::cos(angle)};

		if (glm::dot(direction, visionDirection) <= -0.1f)
			return false;

		auto& info = Game::getCollisionInfo();
		glm::ivec2 positionA = pawn.getPosition();
		glm::ivec2 positionB = point;
		if (positionA == positionB)
			return true;
		if (positionA.x == positionB.x)
		{
			int minY = glm::min(positionA.y, positionB.y);
			int maxY = glm::max(positionA.y, positionB.y);

			for (int i = minY; i < maxY; i++)
				if (info[glm::ivec2{positionA.x, i}] != SquareType::EmptySquare)
					return false;

			return true;
		}
		else if (positionA.y == positionB.y)
		{
			int minX = glm::min(positionA.x, positionB.x);
			int maxX = glm::max(positionA.x, positionB.x);

			for (int i = minX; i < maxX; i++)
				if (info[glm::ivec2{i, positionA.y}] != SquareType::EmptySquare)
					return false;

			return true;
		}
		return false;
	}

	bool atCrossroad(MovingEntity& pawn)
	{
		glm::vec2 position = pawn.getPosition();
		return glm::length(closestCenter(position) - position) < 0.02;
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

	if (!moveAI(pawn, mShortTermGoal))
	{
		mShouldIdle = true;
		return;
	}

	auto *map = Game::get();
	auto& Hero = map->getHero();
	mPawnSeesPlayer = checkVisibility(pawn, Hero.getPosition());
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

	mPawnSeesPlayer = false;
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
	auto* map = Game::get();
	auto& Hero = map->getHero();
	auto heroPosition = Hero.getPosition();
	mPawnSeesPlayer = checkVisibility(pawn, heroPosition);
	if (mPawnSeesPlayer)
		mLastSeenPlayer = closestCenter(Hero.getPosition());
	if (mLastSeenPlayer == glm::vec2(0.f))
		mIsConfused = true;
	else
	{
		bool atDestination = !moveAI(pawn, mLastSeenPlayer);
		mIsConfused = !mPawnSeesPlayer && atDestination;
	}
}

bool ChaseState::transition(const ConfusedState&)
{
	return mIsConfused;
}

void ChaseState::onEntry(MovingEntity& Pawn, float DeltaTime /*= 0*/)
{
	mPawnSeesPlayer = true;
	mIsConfused = false;
	mLastSeenPlayer = glm::vec3(0);
}
/* Chase state -  end */

/* Chase state - start */
bool ConfusedState::transition(const PatrolState&)
{
	return (Game::getCurrentTime() - mConfusionStarted) >= M_PI;
}

bool ConfusedState::transition(const ChaseState&)
{
	return mPawnSeesPlayer;
}

void ConfusedState::onTick(MovingEntity& pawn, float DeltaTime /*= 0*/)
{
	float timePassed = Game::getCurrentTime() - mConfusionStarted;
	pawn.setAngle(initialAngle + glm::sin(timePassed * 2));

	auto* map = Game::get();
	auto& Hero = map->getHero();
	auto heroPosition = Hero.getPosition();
	mPawnSeesPlayer = checkVisibility(pawn, heroPosition);
}

void ConfusedState::onEntry(MovingEntity& pawn, float DeltaTime /*= 0*/)
{
	mConfusionStarted = Game::getCurrentTime();
	initialAngle = pawn.getAngle();
}
/* Confused state -  end */
