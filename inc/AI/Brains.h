#pragma once
#include "StateMachine/StateMachine.h"

namespace bm {
	struct PatrolState;

	struct IdleState : public State {
		using State::transition;
		bool transition(const PatrolState&);
		void onTick(float DeltaTime = 0);
	private:
		float m_IdlingTime;
	};

	struct PatrolState : public State {
		void onTick(float DeltaTime = 0);
	};

	struct ChaseState : public State {};

	typedef SM<IdleState, PatrolState, ChaseState> BrainsSM;
	class Brains : public BrainsSM
	{
	public:
		Brains()
		{
			BM_LOG("%d", sizeof(Brains));
		}
	};
}
