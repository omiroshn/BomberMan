#pragma once
#include "Core.hpp"
#include <tuple>

namespace bm {
	struct Event
	{
	};

	struct State
	{
		unsigned char uid;
		void onTick(float DeltaTime = 0) {
			//BM_CAT_WARN(StateMachine, "Base tick. ID:%d", uid);
		};
		void onEntry() {}
		void onExit() {}
		void onEvent(const Event&) {}

		template<typename T>
		bool transition(const T&) { return false; }
	};

	template <typename... Ts>
	class SM {
	public:
		SM() : m_CurrentState(0)
		{
			init();
		}

		void tick(float DeltaTime = 0) {
			checkTransitions();
			tickCurrentState(DeltaTime);
		}

		template <typename T>
		void setState()
		{
			setStateImpl<T, 0, Ts...>();
		}

	private:
		std::tuple<Ts...>	m_States;
		unsigned char		m_CurrentState : 5;
		unsigned char		_Reserved : 3;

		void init()
		{
			setStateIds<0, Ts...>();
		}

		template <unsigned char N, typename T, typename... Args>
		auto dispatchTick(float DeltaTime)
		{
			if (m_CurrentState == N)
				return std::get<N>(m_States).onTick(DeltaTime);
			if constexpr (sizeof...(Args) > 0)
				dispatchTick<N + 1, Args...>(DeltaTime);
		}

		template<unsigned char N, typename T, typename... Args>
		void setStateIds()
		{
			std::get<N>(m_States).uid = N;
			if constexpr (sizeof...(Args) > 0)
				setStateIds<N + 1, Args...>();
		}

		template <typename T, unsigned char N, typename A, typename... Args>
		void setStateImpl()
		{
			if (std::is_same<T, A>())
			{
				m_CurrentState = N;
				return;
			}
			if constexpr (sizeof...(Args) > 0)
				setStateImpl<T, N + 1, Args...>();
		}

		template <unsigned char N, typename T, typename... Args>
		auto dispatchTransition()
		{
			if (m_CurrentState == N)
				return dispatchTransitionInner<N, 0, Ts...>();
			if constexpr (sizeof...(Args) > 0)
				dispatchTransition<N + 1, Args...>();
		}

		void tickCurrentState(float DeltaTime) { dispatchTick<0, Ts...>(DeltaTime); }

		void checkTransitions() {
			dispatchTransition<0, Ts...>();
		}

		template<unsigned char Current, unsigned char N, typename T, typename... Args>
		auto dispatchTransitionInner()
		{
			const auto &StateToCheck = std::get<N>(m_States);
			if (Current != N && std::get<Current>(m_States).transition(StateToCheck))
			{
				m_CurrentState = N;
				return;
			}
			if constexpr (sizeof...(Args) > 0)
				dispatchTransitionInner<Current, N + 1, Args...>();
		}
	};
}
