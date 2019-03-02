
#ifndef BOMBERMAN_ENTITY_H
#define BOMBERMAN_ENTITY_H

class Entity
{
public:
	Entity(float x, float y);
	~Entity();
	float GetX() const;
	float GetY() const;
	void  SetX(float x);
	void  SetY(float y);

private:
	float mX = 0.f;
	float mY = 0.f;
};

#endif
