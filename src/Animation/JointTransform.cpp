//
// Created by Vadim on 2019-04-05.
//

#include "Animation/JointTransform.hpp"


JointTransform::JointTransform(glm::vec3 position, Quaternion rotation)
: mPosition(position), mRotation(rotation)
{
}

glm::mat4 JointTransform::getLocalTransform()
 {
     glm::mat4 matrix = glm::mat4(1.0f);
     glm::translate(matrix, mPosition);

     matrix *= mRotation.toRotationMatrix();
    return matrix;
}

JointTransform JointTransform::interpolate(JointTransform frameA, JointTransform frameB, float progression)
{
    glm::vec3 pos = interpolate(frameA.mPosition, frameB.mPosition, progression);
    Quaternion rot = Quaternion::interpolate(frameA.mRotation, frameB.mRotation, progression);
    return JointTransform(pos, rot);
}

glm::vec3 JointTransform::interpolate(glm::vec3 start, glm::vec3 end, float progression)
{
        float x = start.x + (end.x - start.x) * progression;
        float y = start.y + (end.y - start.y) * progression;
        float z = start.z + (end.z - start.z) * progression;
        return glm::vec3(x, y, z);
}
