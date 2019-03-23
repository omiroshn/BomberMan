#pragma once
#include "StateMachine/StateMachine.h"
#include "Core.hpp"
#include "glm/vec2.hpp"

class MovingEntity;

/* It's better to provide forward declarations so that any type knows of any other type */
struct IdleState;
struct PatrolState;

/**
 * Idle state
 *
 * Can be used in any Controller. Makes pawn stay in place for 1s.
 */
struct IdleState : public State {
	using State::transition;
	bool transition(const PatrolState&);

	void onEntry(MovingEntity&);

private:
	float m_TransitionToPatrol;
};

/**
 * Patrol state
 *
 * Can be used in any Controller. Makes pawn walk through the maze randomly.
 */
struct PatrolState : public State {
	/** State interface */
	using State::transition;
	bool transition(const IdleState&);
	void onTick(MovingEntity&, float DeltaTime = 0);
	void onEntry(MovingEntity&, float DeltaTime = 0);

	glm::vec2 mCurrentDirection;
	bool mShouldIdle;
};

struct ChaseState : public State {};

typedef SM<MovingEntity, IdleState, PatrolState> BalloonController;

class AIController : public bm::Tickable
{
public:
	void addBalloon(MovingEntity&);
private:
	std::vector<BalloonController> mBalloonControllers;
	virtual void tick(float DeltaTime) override;
};
