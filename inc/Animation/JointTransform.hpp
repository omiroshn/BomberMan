//
// Created by Vadim on 2019-04-05.
//

#ifndef BOMBERMAN_JOINTTRANSFORM_HPP
#define BOMBERMAN_JOINTTRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Quaternion.hpp"

class JointTransform
{
public:
    glm::vec3 mPosition;
    Quaternion mRotation;
    JointTransform(glm::vec3 position, Quaternion rotation);
    glm::mat4 getLocalTransform();
    static JointTransform interpolate(JointTransform frameA, JointTransform frameB, float progression);
    static glm::vec3 interpolate(glm::vec3 start, glm::vec3 end, float progression);
};

#endif //BOMBERMAN_JOINTTRANSFORM_HPP
