
#ifndef BOMBERMAN_ENTITY_H
#define BOMBERMAN_ENTITY_H

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

class Entity
{
public:
	Entity(float x = 0, float y = 0, float angle = 0);
	Entity(glm::vec2 aPosition = {0,0}, float angle = 0);
	~Entity();

	/** Changes position of Entity by offset */
	void		Move(glm::vec2);

	glm::vec2	GetPosition() const;
	void		SetPosition(glm::vec2 position);
	float		GetAngle() const;
	void		SetAngle(float angle);
	glm::mat4	GetModelMatrix()const;

	void		Debug();

	/** compatibility with existing code */
	float GetX() const;
	float GetY() const;
	void  SetX(float x);
	void  SetY(float y);

private:
	glm::vec2	mPosition;
	float		mAngle;

	/** implementation details */
	unsigned	_uid;
	static unsigned _entity_counter;
};

#endif
