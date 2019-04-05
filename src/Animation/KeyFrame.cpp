//
// Created by Vadim on 2019-04-05.
//

#include "Animation/KeyFrame.hpp"


KeyFrame::KeyFrame(float timeStamp, std::map<std::string, JointTransform> const& jointKeyFrames)
: mTimeStamp(timeStamp), mPose(jointKeyFrames)
{
}

float KeyFrame::getTimeStamp()
{
    return mTimeStamp;
}

std::map<std::string, JointTransform> KeyFrame::getJointKeyFrames()
{
    return mPose;
}

