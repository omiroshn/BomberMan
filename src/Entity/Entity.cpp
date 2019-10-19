
#include <Entity/Entity.h>
#include <glm/ext/matrix_transform.hpp>
#include "imgui.h"
#include "Game.hpp"

/** static variable */
unsigned Entity::sEntity_counter = 0;

Entity::Entity(glm::vec2 position, float angle) :
	mPosition(position),
	mAngle(angle),
	mUid(sEntity_counter++),
	mScale(1),
	mDead(false)
{}


Entity::~Entity() {}

glm::vec2	Entity::getPosition() const				{ return mPosition; }
glm::vec3   Entity::getPosition3D() const           { return glm::vec3(mPosition.x, 0, mPosition.y); }
float		Entity::getAngle() const				{ return mAngle; }
float		Entity::getScale() const				{ return mScale; }
bool		Entity::isDead() const					{ return mDead; }

void		Entity::setPosition(glm::vec2 position)	{ mPosition = position; }
void		Entity::setAngle(float radians)			{ mAngle = radians; }
void		Entity::setScale(float value)			{ mScale = value; }

void		Entity::kill()
{
	mDead = true;
}

void Entity::move(glm::vec2 offset)
{
	mPosition += offset;
}

glm::mat4 Entity::getModelMatrix() const
{
	return glm::scale(glm::rotate(glm::translate(glm::mat4(1), glm::vec3(mPosition.x, 0, mPosition.y)), mAngle, glm::vec3(0,1,0)), glm::vec3(mScale));
}

void Entity::debug()
{
#if DEBUG
	ImGui::Text("Entity_%d:", mUid);
	ImGui::DragFloat2("Position", &mPosition[0], 0.1f);
	ImGui::SliderAngle("Angle", &mAngle, -180, 180);
#endif
}
