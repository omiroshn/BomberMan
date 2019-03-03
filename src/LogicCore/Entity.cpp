
#include <LogicCore/Entity.h>
#include <glm/ext/matrix_transform.hpp>
#include "imgui.h"

/** static variable */
unsigned Entity::_entity_counter = 0;

Entity::Entity(glm::vec2 position, float angle) :
	mPosition(position),
	mAngle(angle),
	_uid(_entity_counter++)
{}

Entity::Entity(float x, float y, float angle) : Entity(glm::vec2(x,y), angle) {}

Entity::~Entity() {}

/** self-explanatory */
glm::vec2	Entity::GetPosition() const				{ return mPosition; }
float		Entity::GetAngle() const				{ return mAngle; }
void		Entity::SetPosition(glm::vec2 position)	{ mPosition = position; }
void		Entity::SetAngle(float angle)			{ mAngle = angle; }

/** changes the position of Entity by offset */
void Entity::Move(glm::vec2 offset)
{
	mPosition += offset;
}

/** calculates ModelMatrix with correct rotation end translation on the fly.(Doesn't store it, but we may add this later) */
glm::mat4 Entity::GetModelMatrix() const
{
	return glm::rotate(glm::translate(glm::mat4(1), glm::vec3(mPosition.x, 0, mPosition.y)), mAngle, glm::vec3(0,1,0));
}

/** call it from anywhere in game loop, and you will have accessors for all major variables */
void Entity::Debug()
{
	ImGui::Text("Entity_%d:", _uid);
	ImGui::DragFloat2("Position", &mPosition[0], 0.1f);
	ImGui::SliderAngle("Angle", &mAngle);
}

// left for compatibility reasons
float Entity::GetX() const { return mPosition.x; }
float Entity::GetY() const { return mPosition.y; }
void Entity::SetX(float x) { mPosition.x = x; }
void Entity::SetY(float y) { mPosition.y = y; }
