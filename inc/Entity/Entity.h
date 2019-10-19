
#ifndef BOMBERMAN_ENTITY_H
#define BOMBERMAN_ENTITY_H

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
/*!
\brief The class from which almost everything on screen derives.

It gives common functionality to everyone.
It's mostly a container for position/orientation in the
scene + some functionality that happens to useful to every Entity.
*/
class Entity
{
public:
	Entity(glm::vec2 aPosition = {0,0}, float angle = 0);
	~Entity();

	Entity(Entity const&) = delete;
    Entity& operator=(Entity const&) = delete;
/*!
\brief changes the position of Entity by offset
\param[in] offset How much translation to apply
*/
	void		move(glm::vec2 offset);

/*!
\brief calculates ModelMatrix with correct rotation and translation on the fly. Useful for rendering.

\return matrix representing transformation of this Entity.
*/
	glm::mat4	getModelMatrix()const;
/*!
\return 2D Position of this Entity on map. Useful for collision detection and game logic.
*/
	glm::vec2	getPosition() const;
/*!
\return 3D Position of this Entity on map. Useful for rendering.
*/
	glm::vec3	getPosition3D() const;
/*!
\return Angle of rotation for this entity. Useful for both rendering and game logic.
*/
	float		getAngle() const;
/*!
\return Scale of this entity. Useful for rendering.
*/
	float		getScale() const;
/*!
\return Whether this Entity was marked as dead.
*/
	bool		isDead() const;
/*!
\brief Sets the position of this Entity. Useful for rendering.
*/
	void		setPosition(glm::vec2 position);
/*!
\brief Sets the angle of rotation of this Entity. Useful for rendering.
*/
	void		setAngle(float radians);
/*!
\brief Sets the scale of this Entity. Useful for rendering.
*/
	void		setScale(float value);
/*!
\brief Marks this Entity as dead.
*/
	void		kill();
/*!
\brief Adds this entity info to Debug window.
Call it from anywhere in game loop, and you will have accessors for all major variables
*/
	void		debug();
private:
	glm::vec2	mPosition;
	float		mAngle;
	unsigned	mUid;
	float		mScale;

	static unsigned sEntity_counter;

protected:
	bool		mDead = false;
};

#endif
