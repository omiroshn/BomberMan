#pragma once

#include "Entity.h"
#include "StateMachine/StateMachine.h"

struct Spawning;
struct Counting;
struct Exploding;

class Bomb : public Entity
{
public:
	Bomb(glm::vec2 pos, int explosionStrength);
	~Bomb();
	int mExplosionStrength;
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

	void onTick(Bomb& bomb, float DeltaTime);
	void onEntry(Bomb& bomb);

	float mTimeToExplode;
};

typedef SM<Bomb, Spawning, Counting> BombSM;
