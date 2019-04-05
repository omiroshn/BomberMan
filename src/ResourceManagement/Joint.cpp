#include "ResourceManagement/Joint.hpp"

Joint::Joint(int index, std::string const &name, glm::mat4 bindLocalTransform)
: mIndex(index), mName(name), mLocalBindTransform(bindLocalTransform)
{
}

void Joint::addChild(Joint child)
{
    mChildren.push_back(child);
}

glm::mat4 Joint::getAnimatedTransform()
{
    return mAnimatedTransform;
}

void Joint::setAnimationTransform(glm::mat4 animationTransform)
{
    mAnimatedTransform = animationTransform;
}

glm::mat4 Joint::getInverseBindTransform()
{
    return mInverseBindTransform;
}

void Joint::calcInverseBindTransform(glm::mat4 parentBindTransform)
{
    glm::mat4 bindTransform = parentBindTransform * mLocalBindTransform;
    mInverseBindTransform = glm::inverse(bindTransform);
    for (Joint child : mChildren)
    {
        child.calcInverseBindTransform(bindTransform);
    }
}