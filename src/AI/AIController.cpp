#if 1
#include "AI/AIController.h"
#include "Game.hpp"

std::vector<BalloonController> AIController::mBalloonControllers;

/** AI utility functions */
namespace
{
	enum class EDirection : unsigned char {
		NONE = 0,
		DOWN = 1 << 0,
		RIGHT = 1 << 1,
		UP = 1 << 2,
		LEFT = 1 << 3
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
		return collision[position + glm::vec2(.5)] == SquareType::EmptySquare;
	}

	inline bool isCentered(Entity& entity)
	{
		const glm::vec2 Center = glm::round(entity.getPosition());
		const glm::bvec2 Equal = glm::equal(Center, entity.getPosition());
		return Equal.x & Equal.y;
	}

	EDirection getAvailableDirections(Entity& entity) {
		union {
			unsigned char mask;
			EDirection result;
		};
		result = EDirection::NONE;
		if (isEmpty(entity.getPosition() + glm::vec2(0,1)))
			mask |= (unsigned char)EDirection::DOWN;
		if (isEmpty(entity.getPosition() + glm::vec2(1,0)))
			mask |= (unsigned char)EDirection::RIGHT;
		if (isEmpty(entity.getPosition() + glm::vec2(0,-1)))
			mask |= (unsigned char)EDirection::UP;
		if (isEmpty(entity.getPosition() + glm::vec2(-1,0)))
			mask |= (unsigned char)EDirection::LEFT;
		return result;
	}

	EDirection randomDir() {
		union {
			unsigned char mask;
			EDirection result;
		};
		mask = 1 << (std::rand() & 3);
		return result;
	}

	glm::vec2 pickDirection(Entity& pawn)
	{
		union {
			unsigned char mask;
			EDirection result;
		};
		result = getAvailableDirections(pawn);
		if (result == EDirection::NONE)
			return glm::vec2(0);
		unsigned char i = 1 << (std::rand() & 3);
		while (true)
		{
			if (i > 8)
				i = 1;
			if ((unsigned char)result & i)
				return directionToVector((EDirection)((unsigned char)result & i));
			i <<= 1;
		}
		return glm::vec2(0,1);
	}
}

/* Idle state start */
void IdleState::onEntry(MovingEntity&)		   { m_TransitionToPatrol = Game::getCurrentTime() + 1.f; }
bool IdleState::transition(const PatrolState&) { return m_TransitionToPatrol <= Game::getCurrentTime(); }
/* Idle state end */



/* Patrol state start */
void PatrolState::onTick(MovingEntity& pawn, float DeltaTime)
{
	const glm::bvec2 NotZeroVec = glm::notEqual(mCurrentDirection, glm::vec2(0));
	const bool HasDirection = NotZeroVec.x || NotZeroVec.y;
	if (HasDirection && isEmpty(pawn.getPosition() + mCurrentDirection))
	{
		pawn.AddAcceleration(mCurrentDirection * 60.f);
		return;
	}
	if (!isCentered(pawn))
	{
		const glm::vec2 NearestCenter = glm::round(pawn.getPosition());
		const glm::vec2 ToCenter = NearestCenter - pawn.getPosition();
		if (glm::length(ToCenter) < 0.05)
			pawn.move(ToCenter);
		else
			pawn.AddAcceleration(glm::normalize(ToCenter) * 30.f);
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
/* Patrol state end */
#endif


void AIController::addBalloon(MovingEntity& Balloon)
{
	mBalloonControllers.emplace_back(Balloon);
}

void AIController::tick(float DeltaTime)
{
	for (auto& It : mBalloonControllers)
		It.tick(DeltaTime);
}
