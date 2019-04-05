//
// Created by Vadim on 2019-04-05.
//

#include "Animation/Animation.hpp"


Animation::Animation(float lengthInSeconds, std::vector<KeyFrame> const &frames)
:mLength(lengthInSeconds), mKeyFrames(frames)
{
}

float Animation::getLength() const
{
    return mLength;
}

std::vector<KeyFrame> Animation::getKeyFrames() const
{
    return mKeyFrames;
}
