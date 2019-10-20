#include "Entity/MovingEntity.h"
#include "imgui.h"
#include "Game.hpp"
#include "ResourceManagement/MusicPlayer.hpp"
#include <assimp/types.h>
#include <glm/common.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <iostream>

/** Numbers are tweakable */
float MovingEntity::_Friction = 17;
float MovingEntity::_MaxVelocity = 4.f;
float MovingEntity::_MaxAcceleration = 60.f;

float angleBetween(glm::vec2 vec, glm::vec2 target)
{
	float theta = atan2(vec.y, vec.x) - atan2(target.y, target.x);
	return theta;
}

MovingEntity::MovingEntity(glm::vec2 position, float angle, glm::vec2 velocity, glm::vec2 acceleration) :
	Entity(position, angle),
	mVelocity(velocity),
	mAcceleration(acceleration)
{}

float MovingEntity::GetSpeed() const
{
	const glm::vec2 ElementsSquared = mVelocity * mVelocity;
	return sqrt(ElementsSquared.x + ElementsSquared.y);
}

void MovingEntity::AddVelocity(glm::vec2 velocityOffset)
{
	mVelocity += velocityOffset;
}

bool MovingEntity::isDeadForAwhile() const
{
	return mDead && Game::get()->getCurrentTime() > (mTimeOfDeath + 2.f);
}

void MovingEntity::kill()
{
	MUSIC_PLAYER.playSound("ugh");
	mAnimation.setTime(0.0);
	mTimeOfDeath = Game::get()->getCurrentTime();
	Entity::kill();
}

void MovingEntity::AddAcceleration(glm::vec2 accelerationOffset)
{
	mAcceleration += accelerationOffset;
}

void MovingEntity::debug()
{
#if DEBUG
	Entity::debug();
	ImGui::DragFloat2("Velocity", &mVelocity[0]);
	ImGui::DragFloat2("Acceleration", &mAcceleration[0]);
#endif
}

void MovingEntity::debugMovement()
{
#if DEBUG
	ImGui::Text("MovableEntity setting:");
	ImGui::SliderFloat("Friction", &_Friction, 0.5, 20);
	ImGui::SliderFloat("MaxVelocity", &_MaxVelocity, 0, 20);
	ImGui::SliderFloat("MaxAcceleration", &_MaxAcceleration, 0, 150);
#endif
}

void MovingEntity::animate(float DeltaTime)
{
    mAnimation.tick(DeltaTime);
	if (mDead)
	{
		mAnimation.setType(AnimationType::Dying);
		return;
	}

    if (GetSpeed() >= 0.2)
        mAnimation.setType(AnimationType::Running);
    else
        mAnimation.setType(AnimationType::Idle);
}

void MovingEntity::rotate(float DeltaTime)
{
    float angle = getAngle();
	float desiredAngle = angleBetween(glm::vec2{0, 1}, mVelocity);

	aiQuaternion currentRot(angle, 0, 0);
	aiQuaternion desiredRot(desiredAngle, 0, 0);

	aiQuaternion resultingRot;
	aiQuaternion::Interpolate(resultingRot, currentRot, desiredRot, 4 * DeltaTime);

	aiVector3D result = resultingRot.Rotate(aiVector3D{0, 0, 1});

	float resultingAngle = angleBetween(glm::vec2{0, 1}, glm::vec2(result.x, result.z));
	setAngle(resultingAngle);
}

/** Euler integration + some friction. */
void MovingEntity::tick(float DeltaTime)
{
    animate(DeltaTime);

	debug();

	if (mDead)
		return;

    if (mAcceleration == glm::vec2(0.f) && mVelocity == glm::vec2(0.f))
        return;
    rotate(DeltaTime);
	mAcceleration = glm::clamp(mAcceleration, -_MaxAcceleration, _MaxAcceleration);
	mVelocity += mAcceleration * DeltaTime;
	glm::vec2 dampingVector = mVelocity * _Friction * DeltaTime;
	float damping = glm::length(dampingVector);
	float speed = GetSpeed();
	if (damping > speed)
		dampingVector = (dampingVector / damping) * speed;
	mVelocity -= dampingVector;
	mVelocity = glm::clamp(mVelocity, -_MaxVelocity, _MaxVelocity);
	move(mVelocity * DeltaTime);
	mAcceleration = glm::vec2(0.f);
}

Animation & MovingEntity::getAnimation()
{
    return mAnimation;
}

