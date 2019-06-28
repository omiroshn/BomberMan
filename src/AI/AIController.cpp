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

	glm::vec2 pickDirection(Entity& pawn)
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
}
/** AI utility functions - end */

/* Idle state -  start */
void IdleState::onEntry(MovingEntity&)		   { m_TransitionToPatrol = Game::getCurrentTime() + 1.f; }
bool IdleState::transition(const PatrolState&) { return m_TransitionToPatrol <= Game::getCurrentTime(); }
/* Idle state -  end */



/* Patrol state - start */
void PatrolState::onTick(MovingEntity& pawn, float DeltaTime)
{
	const glm::bvec2 NotZeroVec = glm::notEqual(mCurrentDirection, glm::vec2(0));
	const bool HasDirection = NotZeroVec.x || NotZeroVec.y;

///		pawn.AddAcceleration(mCurrentDirection * 60.f);
	if (HasDirection && isEmpty(pawn.getPosition() + mCurrentDirection))
	{
		return;
	}
	else
		mShouldIdle = true;
}
bool PatrolState::transition(const IdleState&)
{
	return mShouldIdle;
}
void PatrolState::onEntry(MovingEntity& Pawn, float DeltaTime /*= 0*/)
{
	mCurrentDirection = pickDirection(Pawn);
	mShouldIdle = false;
}
/* Patrol state -  end */
