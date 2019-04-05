//
// Created by Vadim on 2019-04-05.
//

#ifndef BOMBERMAN_QUATERNION_HPP
#define BOMBERMAN_QUATERNION_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Quaternion
{
public:
    Quaternion(float x, float y, float z, float w);
    void normalize();
    glm::mat4 toRotationMatrix();
    static Quaternion fromMatrix(glm::mat4 matrix);
    static Quaternion interpolate(Quaternion a, Quaternion b, float blend);
    float mX;
    float mY;
    float mZ;
    float mW;
};

#endif //BOMBERMAN_QUATERNION_HPP
