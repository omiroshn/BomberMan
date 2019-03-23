#if 1
#include "AI/AIController.h"
#include "Game.hpp"

/** AI utility functions */
namespace
{
	enum class EDirection : unsigned char
	{ DOWN, RIGHT, UP, LEFT, COUNT };
	const float directionToAngle[EDirection::COUNT] = {0, 90, 180, 270};
}

/* Idle state start */
void IdleState::onEntry(MovingEntity&)		   { m_TransitionToPatrol = Game::getCurrentTime() + 1.f; }
bool IdleState::transition(const PatrolState&) { return m_TransitionToPatrol >= Game::getCurrentTime(); }
/* Idle state end */



/* Patrol state start */
void PatrolState::onTick(MovingEntity&, float DeltaTime)
{
	BM_CAT_LOG(StateMachine, "Patrol Tick. ID:%d", uid);
}
bool PatrolState::transition(const IdleState&)
{
	return mShouldIdle;
}
/* Patrol state end */
#endif

void PatrolState::onEntry(MovingEntity& Pawn, float DeltaTime /*= 0*/)
{
	mShouldIdle = false;
	
}

void AIController::addBalloon(MovingEntity& Balloon)
{
	mBalloonControllers.emplace_back(Balloon);
}
