#pragma once
#include "Entity.h"
#include "Core.hpp"
#include "ResourceManagement/Animation.h"

class MovingEntity : public Entity, public Tickable
{
public:
	MovingEntity(glm::vec2 position = { 0,0 }, float angle = 0, glm::vec2 velocity = { 0,0 }, glm::vec2 acceleration = {0,0});

	float		GetSpeed()const;
	glm::vec2	GetVelocity()const;
	void		SetVelocity(glm::vec2);
	void		AddVelocity(glm::vec2);
	glm::vec2	GetAcceleration()const;
	void		SetAcceleration(glm::vec2);
	void		AddAcceleration(glm::vec2);

    Animation const& getAnimation() const;
	void		debug();


	static void	debugMovement();

	/** Tickable interface */
	void		tick(float DeltaTime = 0) override;

private:
    void animate();
    void rotate();
private:
	glm::vec2 mVelocity;
	glm::vec2 mAcceleration;
	static float _Friction;
	static float _MaxVelocity;
	static float _MaxAcceleration;
    Animation    mAnimation;
};
