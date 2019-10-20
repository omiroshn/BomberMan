#version 410 core

const int MAX_BONES = 50;
const int MAX_WEIGHTS = 3;

layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 modelsMatrix;

uniform mat4 lightSpaceMatrix;

uniform mat4 parentTransform;

void main()
{
    mat4 transformModelMat = modelsMatrix * parentTransform;
    gl_Position = lightSpaceMatrix * transformModelMat * vec4(aPos, 1.0);
}
