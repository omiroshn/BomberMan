#pragma once

#include "Entity.h"
#include "StateMachine/StateMachine.h"

struct Spawning;
struct Counting;
struct Exploding;

class Bomb : public Entity
{
public:
	static const float FUSE_TIME;
	static const float SPAWN_TIME;
};

struct Spawning : public State {
	using State::transition;

	bool transition(const Counting&);
	void onTick(Bomb& bomb, float DeltaTime);
	void onEntry(Bomb& bomb);

	float mStartTime;
};

struct Counting : public State {
	using State::transition;

	bool transition(const Exploding&);
	void onTick(Bomb& bomb, float DeltaTime);
	void onEntry(Bomb& bomb);

	float mTimeToExplode;
};

struct Exploding : public State {
	void onTick(Bomb& bomb, float DeltaTime);
	void onEntry(Bomb& bomb);
};
