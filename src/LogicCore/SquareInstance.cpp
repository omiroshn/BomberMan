
#include <LogicCore/SquareInstance.h>

SquareInstance::SquareInstance(float x, float y, SquareType type) :
	Entity({x, y}),
	mType(type)
{

}

SquareInstance::~SquareInstance()
{

}


SquareType SquareInstance::GetType() const
{
	return mType;
}


void SquareInstance::ChangeType(SquareType type)
{
	mType = type;
}
