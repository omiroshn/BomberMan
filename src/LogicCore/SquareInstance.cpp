
#include <LogicCore/SquareInstance.h>

SquareInstance::SquareInstance(float x, float y, SquareType type) :
	mX(x),
	mY(y),
	mType(type)
{

}

SquareInstance::~SquareInstance() {

}

float SquareInstance::GetX() const {
	return mX;
}

float SquareInstance::GetY() const {
	return mY;
}

SquareType SquareInstance::GetType() const {
	return mType;
}

void SquareInstance::SetX(float x) {
	if (x >= 0)
		mX = x;
}

void SquareInstance::SetY(float y) {
	if (y >= 0)
		mY = y;
}

void SquareInstance::ChangeType(SquareType type) {
	mType = type;
}
