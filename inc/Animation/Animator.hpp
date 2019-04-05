//
// Created by Vadim on 2019-04-05.
//

#ifndef BOMBERMAN_ANIMATOR_HPP
#define BOMBERMAN_ANIMATOR_HPP

#include <vector>
#include <memory>
#include <map>
#include "ResourceManagement/Joint.hpp"
#include "ResourceManagement/Model.hpp"
#include "Animation.hpp"
#include "KeyFrame.hpp"

class Animator
{
public:
    Model &mEntity;
    Animation *mCurrentAnimation{nullptr};
    float mAnimationTime{0};

    Animator(Model &entity);
    void doAnimation(Animation *animation);
    void update(float deltaTime);
    void increaseAnimationTime(float deltaTime);
    std::map<std::string, glm::mat4> calculateCurrentAnimationPose();
    void applyPoseToJoints(std::map<std::string, glm::mat4> const & currentPose, Joint joint,  glm::mat4 parentTransform);
    std::vector<KeyFrame> getPreviousAndNextFrames();
    float calculateProgression(KeyFrame previousFrame, KeyFrame nextFrame);
    std::map<std::string, glm::mat4> interpolatePoses(KeyFrame previousFrame, KeyFrame nextFrame, float progression);
};

#endif //BOMBERMAN_ANIMATOR_HPP
