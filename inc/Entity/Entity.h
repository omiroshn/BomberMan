
#ifndef BOMBERMAN_ENTITY_H
#define BOMBERMAN_ENTITY_H

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

class Entity
{
public:
	Entity(glm::vec2 aPosition = {0,0}, float angle = 0);
	~Entity();

	Entity(Entity const&) = delete;
    Entity& operator=(Entity const&) = delete;
	/** Changes position of Entity by offset */
	void		move(glm::vec2);

	glm::vec2	getPosition() const;
	glm::vec3	getPosition3D() const;
	glm::mat4	getModelMatrix()const;
	float		getAngle() const;
	float		getScale() const;
	bool		isDead() const;

	void		setPosition(glm::vec2 position);
	void		setAngle(float radians);
	void		setScale(float value);
	void		kill();

	void		debug();

	/** compatibility with existing code */
	float getX() const;
	float getY() const;
	void  setX(float x);
	void  setY(float y);

private:
	glm::vec2	mPosition;
	float		mAngle;
	unsigned	mUid;
	float		mScale;
	bool		mShouldDie;

	static unsigned sEntity_counter;
};

#endif
