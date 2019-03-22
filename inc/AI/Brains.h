#pragma once
#include "StateMachine/StateMachine.h"
#include "LogicCore/MovingEntity.h"

namespace bm {
	struct PatrolState;

	struct IdleState : public State {
		using State::transition;
		bool transition(const PatrolState&);
		void onEntry(MovingEntity&);
	private:
		float m_TransitionToPatrol;
	};

	struct PatrolState : public State {
		void onTick(float DeltaTime = 0);
	};

	struct ChaseState : public State {};

	typedef SM<MovingEntity, IdleState, PatrolState, ChaseState> BrainsSM;
	class Brains : public BrainsSM
	{
	public:
		Brains() : BrainsSM(*this)
		{
			BM_LOG("%d", sizeof(Brains));
		}
	};
}
