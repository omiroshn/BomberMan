//
// Created by Vadim on 2019-04-05.
//

#include "Animation/Animator.hpp"

Animator::Animator(Model &entity)
: mEntity(entity)
{
}

void Animator::doAnimation(Animation *animation)
{
    mAnimationTime = 0;
    mCurrentAnimation = animation;
}

void Animator::update(float deltaTime)
{
    if (mCurrentAnimation == nullptr) {
        return;
    }
    increaseAnimationTime(deltaTime);
    auto currentPose = calculateCurrentAnimationPose();
    applyPoseToJoints(currentPose, mEntity.getRootJoint(), glm::mat4());
}

void Animator::increaseAnimationTime(float deltaTime)
{
    mAnimationTime += deltaTime;
    if (mAnimationTime > mCurrentAnimation->getLength())
    {
        mAnimationTime = fmodf(mAnimationTime, mCurrentAnimation->getLength());
    }
}

std::map<std::string, glm::mat4> Animator::calculateCurrentAnimationPose()
{
        auto frames = getPreviousAndNextFrames();
        float progression = calculateProgression(frames[0], frames[1]);
        return interpolatePoses(frames[0], frames[1], progression);
}

void Animator::applyPoseToJoints(std::map<std::string, glm::mat4> const & currentPose, Joint joint, glm::mat4 parentTransform)
{
    auto currentLocalTransform = currentPose.at(joint.mName);
    glm::mat4 currentTransform = parentTransform * currentLocalTransform;
    for (auto & childJoint : joint.mChildren)
    {
        applyPoseToJoints(currentPose, childJoint, currentTransform);
    }
    currentTransform *= joint.getInverseBindTransform();
    joint.setAnimationTransform(currentTransform);
}

std::vector<KeyFrame> Animator::getPreviousAndNextFrames()
{
    auto allFrames = mCurrentAnimation->getKeyFrames();
    KeyFrame previousFrame = allFrames[0];
    KeyFrame nextFrame = allFrames[0];
    for (int i = 1; i < allFrames.size(); ++i)
    {
        nextFrame = allFrames[i];
        if (nextFrame.getTimeStamp() > mAnimationTime)
        {
            break;
        }
        previousFrame = allFrames[i];
    }
    return std::vector<KeyFrame> { previousFrame, nextFrame };
}

float Animator::calculateProgression(KeyFrame previousFrame, KeyFrame nextFrame)
{
    float totalTime = nextFrame.getTimeStamp() - previousFrame.getTimeStamp();
    float currentTime = mAnimationTime - previousFrame.getTimeStamp();
    return currentTime / totalTime;
}

std::map<std::string, glm::mat4> Animator::interpolatePoses(KeyFrame previousFrame, KeyFrame nextFrame, float progression)
{
    std::map<std::string, glm::mat4> currentPose;
    auto jointKeyFrames = previousFrame.getJointKeyFrames();
    for (auto& jointName : jointKeyFrames)
    {
        JointTransform previousTransform = previousFrame.getJointKeyFrames().at(jointName.first);
        JointTransform nextTransform = nextFrame.getJointKeyFrames().at(jointName.first);
        JointTransform currentTransform = JointTransform::interpolate(previousTransform, nextTransform, progression);
//        currentPose.emplace(jointName, currentTransform.getLocalTransform());
    }
    return currentPose;
}
