#pragma once

#include "Entity.h"
#include "StateMachine/StateMachine.h"
#include "GL/glew.h"
#include <iostream>
#include <vector>

struct Spawning;
struct Counting;
struct Exploding;
/*!
\brief The class representing bomb.

It emits sparks and serves a purpose of some kind of point
of reference for what we think of as bomb.
*/
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

/*!
\brief Struct representing the state of bomb when it's just spawned and slowly grows.
*/
struct Spawning : public State {
	using State::transition;

	bool transition(const Counting&);
	void onTick(Bomb& bomb, float DeltaTime);
	void onEntry(Bomb& bomb);

	float mStartTime;
};

/*!
\brief Struct representing the state of bomb when it's ticking, counting time to explosion.
*/
struct Counting : public State {
	using State::transition;

	void onTick(Bomb& bomb, float DeltaTime);
	void onEntry(Bomb& bomb);

	float mTimeToExplode;
	float mCountdown;
};

/*!
\brief State machine which controlls how Bomb behaves.
*/
typedef SM<Bomb, Spawning, Counting> BombController;
