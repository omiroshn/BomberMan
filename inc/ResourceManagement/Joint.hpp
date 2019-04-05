//
// Created by Vadim
//

#ifndef BOMBERMAN_JOINT_HPP
#define BOMBERMAN_JOINT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

class Joint
{
public:
    int                 mIndex;
    std::string         mName;
    std::vector<Joint>  mChildren;
    glm::mat4           mAnimatedTransform;
    glm::mat4           mLocalBindTransform;
    glm::mat4           mInverseBindTransform;

    Joint(int index, std::string const &name, glm::mat4 bindLocalTransform);
    void addChild(Joint child);
    glm::mat4 getAnimatedTransform();
    void setAnimationTransform(glm::mat4 animationTransform);
    glm::mat4 getInverseBindTransform();
    void calcInverseBindTransform(glm::mat4 parentBindTransform);
};

#endif //BOMBERMAN_JOINT_HPP
