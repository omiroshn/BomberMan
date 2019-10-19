#pragma once
#include "StateMachine/StateMachine.h"
#include "glm/vec2.hpp"


/* It's better to provide forward declarations so that any type knows of any other type */
struct IdleState;
struct PatrolState;
struct ChaseState;
struct ConfusedState;
class MovingEntity;

/*!
\brief Struct representing the state of entity when it's doing nothing.
*/
struct IdleState : public State {
	using State::transition;

    IdleState();
    ~IdleState();
	IdleState(IdleState const&);
	IdleState& operator=(IdleState const&);
	bool transition(const PatrolState&);

	void onEntry(MovingEntity&);
	void onTick(MovingEntity&, float DeltaTime = 0);

private:
	float m_TransitionToPatrol;
};

/*!
\brief Struct representing the state of entity when it's wandering around the map
Makes pawn walk through the maze randomly.
*/
struct PatrolState : public State {
    PatrolState();
    ~PatrolState();
	PatrolState(PatrolState const&);
	PatrolState& operator=(PatrolState const&);
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

/*!
\brief Struct representing the state of entity when it's seen a player.
Makes pawn chase after the player.
*/
struct ChaseState : public State {
    ChaseState();
    ~ChaseState();
	ChaseState(ChaseState const&);
	ChaseState& operator=(ChaseState const&);

	using State::transition;
	bool transition(const ConfusedState&);
	void onTick(MovingEntity&, float DeltaTime = 0);
	void onEntry(MovingEntity&, float DeltaTime = 0);

	glm::vec2 mLastSeenPlayer;
	bool mPawnSeesPlayer;
	bool mIsConfused;
};

/*!
\brief Struct representing the state of entity when it's lost a player when chasing him.
Makes pawn stay in place trying to find the player.
*/
struct ConfusedState : public State {
    ConfusedState();
    ~ConfusedState();
	ConfusedState(ConfusedState const&);
	ConfusedState& operator=(ConfusedState const&);

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

typedef SM<MovingEntity, IdleState, PatrolState> BalloonController;
typedef SM<MovingEntity, IdleState, PatrolState, ChaseState, ConfusedState> OnilController;
typedef SM<MovingEntity, IdleState, PatrolState> OvapeController;
