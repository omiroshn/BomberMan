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

void bm::IdleState::onTick(float DeltaTime /*= 0*/)
{
	m_IdlingTime += DeltaTime;
	BM_CAT_LOG(StateMachine, "Been Idle for %f. ID:%d", m_IdlingTime, uid);
}

void bm::PatrolState::onTick(float DeltaTime /*= 0*/)
{
	BM_CAT_LOG(StateMachine, "Patrol Tick. ID:%d", uid);
}
