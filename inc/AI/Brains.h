#pragma once
#include "StateMachine/StateMachine.h"

namespace bm {
#if __BOOST_SML_IMPLEMENTATION__
	namespace sml = boost::sml;

	// actions
	const auto send_fin = [] {};

	// states
	class idle;
	class patrol;
	class chase;

	class Brains;

	typedef tick<Brains> BrainsTick;

	bool see_player(const BrainsTick&);

	const auto time_passed = [](const BrainsTick& e) { return true; };

	struct AITT {
		auto operator()() const {
			using namespace sml;
			return make_transition_table(
				*state<idle>  + event<BrainsTick> [time_passed] = state<patrol>,
				state<patrol> + event<BrainsTick> [see_player]  = state<chase>,
				state<chase>  + event<BrainsTick> [!see_player] = state<idle>
			);
		}
	};

	class Brains : sml::sm<AITT>
	{
		bool m_SawPlayer;
	public:
		void Update(float DeltaTime = 0)
		{
			BrainsTick Tmp{DeltaTime, this};
			process_event(Tmp);
		}

		bool GetSawPlayer() const { return m_SawPlayer; }
	};

	bool see_player(const BrainsTick& e)
	{
		return e.UserData->GetSawPlayer();
	}
#else

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

#endif
}
