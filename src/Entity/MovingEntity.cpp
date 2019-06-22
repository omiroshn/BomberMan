#include "Entity/MovingEntity.h"
#include "glm/common.hpp"
#include "imgui.h"

/** Numbers are tweakable */
float MovingEntity::_Friction = 17;
float MovingEntity::_MaxVelocity = 4.f;
float MovingEntity::_MaxAcceleration = 60.f;

#ifndef M_PI_2
# define M_PI_2 1.57079632679
#endif // !M_PI_2


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

void MovingEntity::SetVelocity(glm::vec2 velocity)
{
	mVelocity = velocity;
}

void MovingEntity::AddVelocity(glm::vec2 velocityOffset)
{
	mVelocity += velocityOffset;
}

glm::vec2 MovingEntity::GetVelocity() const
{
	return mVelocity;
}

void MovingEntity::SetAcceleration(glm::vec2 acceleration)
{
	mAcceleration = acceleration;
}

void MovingEntity::AddAcceleration(glm::vec2 accelerationOffset)
{
	mAcceleration += accelerationOffset;
}

void MovingEntity::debug()
{
	Entity::debug();
	ImGui::DragFloat2("Velocity", &mVelocity[0]);
	ImGui::DragFloat2("Acceleration", &mAcceleration[0]);
}

void MovingEntity::debugMovement()
{
	ImGui::Text("MovableEntity setting:");
	ImGui::SliderFloat("Friction", &_Friction, 0.5, 20);
	ImGui::SliderFloat("MaxVelocity", &_MaxVelocity, 0, 20);
	ImGui::SliderFloat("MaxAcceleration", &_MaxAcceleration, 0, 150);
}

glm::vec2 MovingEntity::GetAcceleration() const
{
	return mAcceleration;
}

void MovingEntity::animate()
{
    mAnimation.tick();
    if (GetSpeed() < 1)
        mAnimation.setType(AnimationType::Idle);
    else
        mAnimation.setType(AnimationType::Running);
}

void MovingEntity::rotate()
{
    float angle{0};
    if (abs(mVelocity.x) > 1)
    {
        angle = M_PI_2 * (mVelocity.x > 0 ? 1 : -1);
    }
    if (abs(mVelocity.y) > 1)
    {
        angle = M_PI * (mVelocity.y > 0 ? 0 : -1);
    }
    setAngle(angle);
}

/** Euler integration + some friction. */
void MovingEntity::tick(float DeltaTime)
{
    animate();
    if (mAcceleration == glm::vec2(0.f) && mVelocity == glm::vec2(0.f))
        return;
    rotate();
    mAcceleration -= mAcceleration * _Drag * DeltaTime;
	mAcceleration = glm::clamp(mAcceleration, -_MaxAcceleration, _MaxAcceleration);
	mVelocity += mAcceleration * DeltaTime;
	mVelocity -= mVelocity * _Friction * DeltaTime;
	mVelocity = glm::clamp(mVelocity, -_MaxVelocity, _MaxVelocity);
	move(mVelocity * DeltaTime);
	const float velocityLength = glm::length(mVelocity);
	if (velocityLength < 0.0001f)
		mVelocity = glm::vec2(0.f);
	mAcceleration = glm::vec2(0.f);
}

Animation const& MovingEntity::getAnimation() const
{
    return mAnimation;
}
