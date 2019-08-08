#pragma once
#include "StateMachine/StateMachine.h"
#include "glm/vec2.hpp"

class MovingEntity;

/* It's better to provide forward declarations so that any type knows of any other type */
struct IdleState;
struct PatrolState;
struct ChaseState;
struct ConfusedState;

/**
 * Idle state
 *
 * Makes pawn stay in place for 1s.
 */
struct IdleState : public State {
	using State::transition;
    IdleState();
    ~IdleState();
    //IdleState(IdleState const&) = delete;
    //IdleState& operator=(IdleState const&) = delete;
	bool transition(const PatrolState&);

	void onEntry(MovingEntity&);

private:
	float m_TransitionToPatrol;
};

/**
 * Patrol state
 *
 * Makes pawn walk through the maze randomly.
 */
struct PatrolState : public State {
	/** State interface */
	using State::transition;
	bool transition(const IdleState&);
	bool transition(const ChaseState&);
	void onTick(MovingEntity&, float DeltaTime = 0);
	void onEntry(MovingEntity&, float DeltaTime = 0);

	glm::vec2 mCurrentDirection;
	glm::vec2 mShortTermGoal;
	bool mShouldIdle;
	bool mPawnSeesPlayer;
};

/**
 * Patrol state
 *
 * Makes pawn walk through the maze randomly.
 */
struct ChaseState : public State {
	using State::transition;
	bool transition(const ConfusedState&);
	void onTick(MovingEntity&, float DeltaTime = 0);
	void onEntry(MovingEntity&, float DeltaTime = 0);

	glm::vec2 mLastSeenPlayer;
	bool mPawnSeesPlayer;
	bool mIsConfused;
};

struct ConfusedState : public State {
	using State::transition;
	bool transition(const PatrolState&);
	bool transition(const ChaseState&);
	void onTick(MovingEntity&, float DeltaTime = 0);
	void onEntry(MovingEntity&, float DeltaTime = 0);

	float	mConfusionStarted;
	float	initialAngle;
	bool	mShouldMoveOn;
	bool	mPawnSeesPlayer;
};

typedef SM<MovingEntity, IdleState, PatrolState, ChaseState, ConfusedState> BalloonController;
