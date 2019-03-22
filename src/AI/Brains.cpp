#if 1
#include "AI/Brains.h"
#include "Game.hpp"

bool bm::IdleState::transition(const PatrolState&)
{
	return m_TransitionToPatrol >= Game::getCurrentTime();
}

void bm::IdleState::onEntry(MovingEntity&)
{
	m_TransitionToPatrol = Game::getCurrentTime() + 3;
}

void bm::PatrolState::onTick(float DeltaTime)
{
	BM_CAT_LOG(StateMachine, "Patrol Tick. ID:%d", uid);
}
#endif
