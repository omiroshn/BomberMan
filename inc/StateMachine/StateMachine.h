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

		template<typename T>
		void onTick(T& Owner, float DeltaTime) {};
		template<typename T>
		bool transition(const T&) { return false; }
		template<typename T>
		void onEntry(T& Owner) {}

		void onEvent(const Event&) {}
	};

	template <typename OwnerType,typename... Ts>
	class SM {
	public:
		SM(OwnerType& Owner, unsigned char InitialState = 0) :
			m_Owner(Owner),
			m_CurrentState(InitialState)
		{
			init();
		}

		void tick(float DeltaTime = 0) {
			dispatchTransitions();
			tickCurrentState(DeltaTime);
		}

		template <typename T>
		void setState()
		{
			setStateImpl<T, 0, Ts...>();
		}

	private:
		std::tuple<Ts...>	m_States;
		OwnerType&			m_Owner;
		unsigned char		m_CurrentState : 5;

		void init()
		{
			setStateIds<0, Ts...>();
		}

		template <unsigned char N, typename T, typename... Args>
		auto dispatchTick(float DeltaTime)
		{
			if (m_CurrentState == N)
				return std::get<N>(m_States).onTick(m_Owner, DeltaTime);
			if constexpr (sizeof...(Args) > 0)
				dispatchTick<N + 1, Args...>(DeltaTime);
		}

		template <unsigned char N, typename T, typename... Args>
		auto dispatchEntry()
		{
			if (m_CurrentState == N)
				return std::get<N>(m_States).onEntry(m_Owner);
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
				return setStateImpl<T, N + 1, Args...>();
			else
				BM_CAT_CRITICAL(StateMachine, "setState<>() used with inapropriate state.");
		}

		template <unsigned char N, typename T, typename... Args>
		auto dispatchTransitionsImpl()
		{
			if (m_CurrentState == N)
				return dispatchTransitionInner<N, 0, Ts...>();
			if constexpr (sizeof...(Args) > 0)
				dispatchTransitionsImpl<N + 1, Args...>();
		}

		void tickCurrentState(float DeltaTime) { dispatchTick<0, Ts...>(DeltaTime); }

		void dispatchTransitions() {
			dispatchTransitionsImpl<0, Ts...>();
		}

		template<unsigned char Current, unsigned char N, typename T, typename... Args>
		auto dispatchTransitionInner()
		{
			const auto &StateToCheck = std::get<N>(m_States);
			if (Current != N && std::get<Current>(m_States).transition(StateToCheck))
			{
				m_CurrentState = N;
				dispatchEntry();
				return;
			}
			if constexpr (sizeof...(Args) > 0)
				dispatchTransitionInner<Current, N + 1, Args...>();
		}
	};
}
