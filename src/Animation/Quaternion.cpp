//
// Created by Vadim on 2019-04-05.
//

#include "Animation/Quaternion.hpp"
#include <math.h>

Quaternion::Quaternion(float x, float y, float z, float w)
: mX(x), mY(y), mZ(z), mW(w)
{
        normalize();
}

void Quaternion::normalize()
{
    float mag = sqrt(mW * mW + mX * mX + mY * mY + mZ * mZ);
    mW /= mag;
    mX /= mag;
    mY /= mag;
    mZ /= mag;
}

glm::mat4 Quaternion::toRotationMatrix()
{
    glm::mat4 matrix = glm::mat4();
    float xy = mX * mY;
    float xz = mX * mZ;
    float xw = mX * mW;
    float yz = mY * mZ;
    float yw = mY * mW;
    float zw = mZ * mW;
    float xSquared = mX * mX;
    float ySquared = mY * mY;
    float zSquared = mZ * mZ;

    matrix[0][0] = 1 - 2 * (ySquared + zSquared);
    matrix[0][1]= 2 * (xy - zw);
    matrix[0][2] = 2 * (xz + yw);
    matrix[0][3] = 0;
    matrix[1][0]= 2 * (xy + zw);
    matrix[1][1]= 1 - 2 * (xSquared + zSquared);
    matrix[1][2] = 2 * (yz - xw);
    matrix[1][3]= 0;
    matrix[2][0] = 2 * (xz - yw);
    matrix[2][1] = 2 * (yz + xw);
    matrix[2][2] = 1 - 2 * (xSquared + ySquared);
    matrix[2][3] = 0;
    matrix[3][0] = 0;
    matrix[3][1] = 0;
    matrix[3][2] = 0;
    matrix[3][3] = 1;
    return matrix;
}

Quaternion Quaternion::fromMatrix(glm::mat4 matrix)
{
        float w, x, y, z;
        float diagonal = matrix[0][0] + matrix[1][1]+ matrix[2][2];
        if (diagonal > 0)
        {
            float w4 = sqrt(diagonal + 1.f) * 2.f;
            w = w4 / 4.f;
            x = (matrix[2][1] - matrix[1][2]) / w4;
            y = (matrix[0][2] - matrix[2][0]) / w4;
            z = (matrix[1][0] - matrix[0][1]) / w4;
        }
        else if ((matrix[0][0] > matrix[1][1]) && (matrix[0][0] > matrix[2][2]))
        {
            float x4 = sqrt(1.f + matrix[0][0]- matrix[1][1] - matrix[2][2]) * 2.f;
            w = (matrix[2][1] - matrix[1][2]) / x4;
            x = x4 / 4.f;
            y = (matrix[0][1] + matrix[1][0]) / x4;
            z = (matrix[0][2] + matrix[2][0]) / x4;
        }
        else if (matrix[1][1] > matrix[2][2])
        {
            float y4 = sqrt(1.f + matrix[1][1] - matrix[0][0] - matrix[2][2]) * 2.f;
            w = (matrix[0][2] - matrix[2][0]) / y4;
            x = (matrix[0][1] + matrix[1][0]) / y4;
            y = y4 / 4.f;
            z = (matrix[1][2] + matrix[2][1]) / y4;
        }
        else
        {
            float z4 = sqrt(1.f + matrix[2][2] - matrix[0][0] - matrix[1][1]) * 2.f;
            w = (matrix[1][0] - matrix[0][1]) / z4;
            x = (matrix[0][2] + matrix[2][0]) / z4;
            y = (matrix[1][2] + matrix[2][1]) / z4;
            z = z4 / 4.f;
        }
        return Quaternion(x, y, z, w);
    }

Quaternion Quaternion::interpolate(Quaternion a, Quaternion b, float blend)
{
    Quaternion result(0, 0, 0, 1);
    float dot = a.mW * b.mW + a.mX * b.mX + a.mY * b.mY + a.mZ * b.mZ;
    float blendI = 1.0f - blend;
    if (dot < 0)
    {
        result.mW = blendI * a.mW + blend * -b.mW;
        result.mX = blendI * a.mX + blend * -b.mX;
        result.mY = blendI * a.mY + blend * -b.mY;
        result.mZ = blendI * a.mZ + blend * -b.mZ;
    }
    else
    {
        result.mW = blendI * a.mW + blend * b.mW;
        result.mX = blendI * a.mX + blend * b.mX;
        result.mY = blendI * a.mY + blend * b.mY;
        result.mZ = blendI * a.mZ + blend * b.mZ;
    }
    result.normalize();
    return result;
}
