#pragma once
#include "Entity.h"
#include "Core.hpp"
#include "ResourceManagement/Animation.h"

/*!
\brief Extends the functionality of Entity. Represents all Moving entities in game.
*/
class MovingEntity : public Entity, public Tickable
{
public:
	MovingEntity(glm::vec2 position = { 0,0 }, float angle = 0, glm::vec2 velocity = { 0,0 }, glm::vec2 acceleration = {0,0});
	
	MovingEntity() = delete;
	MovingEntity(MovingEntity const&) = delete;
    MovingEntity& operator=(MovingEntity const&) = delete;
	
/*!
\brief Non-virtual override of Entity::kill
*/
	void		kill();
/*!
\return Whether this Entity was dead for a long time.
*/
	bool		isDeadForAwhile() const;
/*!
\return Speed of this MovingEntity 
*/
	float		GetSpeed()const;
/*!
\brief Adds velocity to this MovingEntity own velocity.
\param[in] velocityOffset Velocity to add.
*/
	void		AddVelocity(glm::vec2 velocityOffset);
/*!
\brief Adds acceleration to this MovingEntity own acceleration.

This is basically the only appropriate way to move
an Entity that will look and feel properly.
\param[in] velocityOffset Velocity to add.
*/
	void		AddAcceleration(glm::vec2);
/*!
\return Reference to Animation for this Entity
*/
    Animation & getAnimation();

/*!
\brief Expands on Entity's debug func. Adds even more fields to debug window.
*/
	void		debug();

/*!
\brief Adds Entity-independant movement variables to debug window.
*/
	static void	debugMovement();

	/** Tickable interface */
	void		tick(float DeltaTime = 0) override;

private:
    void animate(float DeltaTime);
    void rotate(float DeltaTime);
private:
	glm::vec2	mVelocity;
	glm::vec2	mAcceleration;
    Animation   mAnimation;
	float		mTimeOfDeath;

	static float _Friction;
	static float _MaxVelocity;
	static float _MaxAcceleration;
};
