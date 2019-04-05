//
// Created by Vadim on 2019-04-05.
//

#ifndef BOMBERMAN_KEYFRAME_HPP
#define BOMBERMAN_KEYFRAME_HPP

#include <map>
#include <string>
#include "JointTransform.hpp"

class KeyFrame
{
public:
    float                                   mTimeStamp;
    std::map<std::string, JointTransform>   mPose;

    KeyFrame(float timeStamp,std::map<std::string, JointTransform> const& jointKeyFrames);
    float getTimeStamp();
    std::map<std::string, JointTransform> getJointKeyFrames();
};
#endif //BOMBERMAN_KEYFRAME_HPP
