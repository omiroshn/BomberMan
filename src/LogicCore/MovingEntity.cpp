#include "LogicCore/MovingEntity.h"
#include "glm/common.hpp"
#include "imgui.h"

/** Numbers are tweakable */
float MovingEntity::_Friction = 0.014f;
float MovingEntity::_Drag = 0.011f;
float MovingEntity::_MaxVelocity = 40;
float MovingEntity::_MaxAcceleration = 150;

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

void MovingEntity::Debug()
{
	Entity::Debug();
	ImGui::DragFloat2("Velocity", &mVelocity[0]);
	ImGui::DragFloat2("Acceleration", &mAcceleration[0]);
}

void MovingEntity::DebugMovement()
{
	ImGui::Text("MovableEntity setting:");
	ImGui::SliderFloat("Friction", &_Friction, 0, 1);
	ImGui::SliderFloat("Drag", &_Drag, 0, 1);
	ImGui::SliderFloat("MaxVelocity", &_MaxVelocity, 0, 150);
	ImGui::SliderFloat("MaxAcceleration", &_MaxAcceleration, 0, 150);
}

glm::vec2 MovingEntity::GetAcceleration() const
{
	return mAcceleration;
}

/** Euler integration + some friction. */
void MovingEntity::tick(float DeltaTime)
{
	mVelocity += mAcceleration * DeltaTime;
	mVelocity = glm::clamp(mVelocity, -_MaxVelocity, _MaxVelocity);
	mVelocity *= _Friction / DeltaTime;
	mAcceleration *= _Drag / DeltaTime;
	mAcceleration = glm::clamp(mAcceleration, -_MaxAcceleration, _MaxAcceleration);
	Move(mVelocity * DeltaTime);
}
