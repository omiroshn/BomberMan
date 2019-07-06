#pragma once
#include "Core.hpp"
#include <tuple>

struct Event
{
};

struct State
{
	unsigned char uid;

	template<typename T>
	void onTick(T& Owner, float DeltaTime) {};
	template<typename T>
	void onEntry(T& Owner) {}

	template<typename T>
	bool transition(const T&) { return false; }

	void onEvent(const Event&) {}
};

template <typename OwnerType, typename... Ts>
class SM {
	static const uint8_t INVALID_STATE = 255;
public:
	SM() :
		m_CurrentState(INVALID_STATE)
	{
		init();
	}

	void tick(OwnerType& owner, float DeltaTime = 0)
	{
		if (m_CurrentState == INVALID_STATE)
		{
			m_CurrentState = 0;
			dispatchEntry<0, Ts...>(owner);
		}
		tickCurrentState(owner, DeltaTime);
		dispatchTransitions(owner);
	}

	template <typename T>
	void setState()
	{
		setStateImpl<T, 0, Ts...>();
	}

private:
	std::tuple<Ts...>	m_States;
	unsigned char		m_CurrentState;

	void init()
	{
		setStateIds<0, Ts...>();
	}

	template <unsigned char N, typename T, typename... Args>
	auto dispatchTick(OwnerType& owner, float DeltaTime)
	{
		if (m_CurrentState == N)
			return std::get<N>(m_States).onTick(owner, DeltaTime);
		if constexpr (sizeof...(Args) > 0)
			dispatchTick<N + 1, Args...>(owner, DeltaTime);
	}

	template <unsigned char N, typename T, typename... Args>
	auto dispatchEntry(OwnerType& owner)
	{
		if (m_CurrentState == N)
			return std::get<N>(m_States).onEntry(owner);
		if constexpr (sizeof...(Args) > 0)
			dispatchEntry<N + 1, Args...>(owner);
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
			BM_CRITICAL("setState<>() used with inapropriate state.");
	}

	template <unsigned char N, typename T, typename... Args>
	auto dispatchTransitionsImpl(OwnerType& owner)
	{
		if (m_CurrentState == N)
			return dispatchTransitionInner<N, 0, Ts...>(owner);
		if constexpr (sizeof...(Args) > 0)
			dispatchTransitionsImpl<N + 1, Args...>(owner);
	}

	void tickCurrentState(OwnerType& owner, float DeltaTime) { dispatchTick<0, Ts...>(owner, DeltaTime); }

	void dispatchTransitions(OwnerType& owner) {
		dispatchTransitionsImpl<0, Ts...>(owner);
	}

	template<unsigned char Current, unsigned char N, typename T, typename... Args>
	auto dispatchTransitionInner(OwnerType& owner)
	{
		const auto &StateToCheck = std::get<N>(m_States);
		if (Current != N && std::get<Current>(m_States).transition(StateToCheck))
		{
			m_CurrentState = N;
			dispatchEntry<0, Ts...>(owner);
			return;
		}
		if constexpr (sizeof...(Args) > 0)
			dispatchTransitionInner<Current, N + 1, Args...>(owner);
	}
};
