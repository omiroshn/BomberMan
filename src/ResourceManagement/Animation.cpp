//
// Created by Vadym KOZLOV on 2019-06-28.
//

#include "ResourceManagement/Animation.h"

Animation::Animation(AnimationType type) :  mTime{0.0}, mType{type}
{
}

Animation::Animation(double time, AnimationType type) :  mTime{time}, mType{type}
{
}


double Animation::getTime() const
{
    return mTime;
}

void Animation::setTime(double time)
{
    mTime = time;
}

AnimationType Animation::getType() const
{
    return mType;
}

void Animation::setType(AnimationType type)
{
    mType = type;
}

std::string  Animation::getName() const
{
    switch (mType)
    {
        case AnimationType::Running:
            return "run";
        case AnimationType::Dying:
            return "die";
        default:
            return "idle";
    }
}

void         Animation::tick(float DeltaTime)
{
    mTime += DeltaTime;
}