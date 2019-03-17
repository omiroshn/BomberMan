#if 1
#include "AI/Brains.h"

bool bm::IdleState::transition(const PatrolState&)
{
	if (m_IdlingTime > 5)
	{
		m_IdlingTime = 0;
		return true;
	}
	return false;
}

void bm::IdleState::onTick(float DeltaTime)
{
	m_IdlingTime += DeltaTime;
	BM_CAT_LOG(StateMachine, "Been Idle for %f. ID:%d", m_IdlingTime, uid);
}

void bm::PatrolState::onTick(float DeltaTime)
{
	BM_CAT_LOG(StateMachine, "Patrol Tick. ID:%d", uid);
}
#endif
