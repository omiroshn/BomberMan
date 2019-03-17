#pragma once
#include "StateMachine/StateMachine.h"
#include "LogicCore/MovingEntity.h"

namespace bm {
	struct PatrolState;

	struct IdleState : public State {
		using State::transition;
		bool transition(const PatrolState&);
		void onTick(MovingEntity& Owner, float DeltaTime = 0);
	private:
		float m_IdlingTime;
	};

	struct PatrolState : public State {
		void onTick(float DeltaTime = 0);
	};

	struct ChaseState : public State {};

	typedef SM<MovingEntity, IdleState, PatrolState, ChaseState> BrainsSM;
	class Brains : public BrainsSM, public MovingEntity
	{
	public:
		Brains() : BrainsSM(*this)
		{
			BM_LOG("%d", sizeof(Brains));
		}
	};
}
