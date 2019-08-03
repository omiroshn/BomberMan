#pragma once

#include "Entity.h"
#include "StateMachine/StateMachine.h"
#include "GL/glew.h"
#include <iostream>
#include <vector>

struct Spawning;
struct Counting;
struct Exploding;

class Bomb : public Entity
{
public:
	static GLuint						mSparksBuffer;
    static GLuint						mSparksArray;
	int									mExplosionStrength;
	static std::vector<glm::vec4>		mSparksQuads;

	Bomb(glm::vec2 pos, int explosionStrength);
	~Bomb();
	Bomb(Bomb const&) = delete;
    Bomb& operator=(Bomb const&) = delete;
	
	static void bindArrays();
	static void drawSparks(glm::vec4 position);
	static void drawSparksQuadsDeferred(glm::mat4 view, glm::mat4 projection);
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
	float mCountdown;
};

typedef SM<Bomb, Spawning, Counting> BombSM;
