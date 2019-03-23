
#include <Entity/Entity.h>
#include <glm/ext/matrix_transform.hpp>
#include "imgui.h"

/** static variable */
unsigned Entity::sEntity_counter = 0;

Entity::Entity(glm::vec2 position, float angle) :
	mPosition(position),
	mAngle(angle),
	mUid(sEntity_counter++)
{}

Entity::Entity(float x, float y, float angle) : Entity(glm::vec2(x,y), angle) {}

Entity::~Entity() {}

/** self-explanatory */
glm::vec2	Entity::getPosition() const				{ return mPosition; }
glm::vec3   Entity::getPosition3D() const           { return glm::vec3(mPosition.x, 0, mPosition.y); }
float		Entity::getAngle() const				{ return mAngle; }
void		Entity::setPosition(glm::vec2 position)	{ mPosition = position; }
void		Entity::setAngle(float angle)			{ mAngle = angle; }

/** changes the position of Entity by offset */
void Entity::move(glm::vec2 offset)
{
	mPosition += offset;
}

/** calculates ModelMatrix with correct rotation end translation on the fly.(Doesn't store it, but we may add this later) */
glm::mat4 Entity::getModelMatrix() const
{
	return glm::rotate(glm::translate(glm::mat4(1), glm::vec3(mPosition.x, 0, mPosition.y)), mAngle, glm::vec3(0,1,0));
}

/** call it from anywhere in game loop, and you will have accessors for all major variables */
void Entity::debug()
{
	ImGui::Text("Entity_%d:", mUid);
	ImGui::DragFloat2("Position", &mPosition[0], 0.1f);
	ImGui::SliderAngle("Angle", &mAngle);
}

// left for compatibility reasons
float Entity::getX() const { return mPosition.x; }
float Entity::getY() const { return mPosition.y; }
void Entity::setX(float x) { mPosition.x = x; }
void Entity::setY(float y) { mPosition.y = y; }
