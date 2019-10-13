#pragma once
#include <tuple>
#include <stdint.h>

/*!
\brief Represents current state of the StateMachine.
Holds all necessary info about current state in itself.
*/
struct State
{
/*!
\brief Called every frame for this state and lets him do everything he needs to.
*/
	template<typename T>
	void onTick(T&, float) {};
/*!
\brief Called every time this state is entered. Lets this state setup it's data.
*/
	template<typename T>
	void onEntry(T&) {}
/*!
\brief Called every frame to see if this State need to be changed to another.
*/
	template<typename T>
	bool transition(const T&) { return false; }
};

/*!
\brief Compile-time generated StateMachine. Useful for implementing AI.

Holds all of it's states in itself.
It controlls 1 specific type of Entity per template instatiation.
*/
template <typename OwnerType, typename... Ts>
class SM {
	static const uint8_t INVALID_STATE = 255;
public:

/*!
\brief Called every frame.
This is where current state is ticked and transitions are dispatched.
*/
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

private:
	std::tuple<Ts...>	m_States;
	unsigned char		m_CurrentState = INVALID_STATE;

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

	template <unsigned char N, typename T, typename... Args>
	auto dispatchTransitionsImpl(OwnerType& owner)
	{
		if (m_CurrentState == N)
			return dispatchTransitionInner<N, 0, Ts...>(owner);
		if constexpr (sizeof...(Args) > 0)
			dispatchTransitionsImpl<N + 1, Args...>(owner);
	}

	void tickCurrentState(OwnerType& owner, float DeltaTime)
	{
		dispatchTick<0, Ts...>(owner, DeltaTime);
	}

	void dispatchTransitions(OwnerType& owner)
	{
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
