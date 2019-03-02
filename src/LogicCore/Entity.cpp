
#include <LogicCore/Entity.h>

Entity::Entity(float x, float y) :
	mX(x),
	mY(y)
{

}

Entity::~Entity() {

}

float Entity::GetX() const {
	return mX;
}

float Entity::GetY() const {
	return mY;
}


void Entity::SetX(float x) {
	if (x >= 0)
		mX = x;
}

void Entity::SetY(float y) {
	if (y >= 0)
		mY = y;
}

