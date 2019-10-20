#version 410 core

const int MAX_BONES = 50;
const int MAX_WEIGHTS = 3;

layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 modelsMatrix;

layout(location = 7) in ivec3 bonesID;
layout(location = 8) in vec3 weights;

uniform mat4 lightSpaceMatrix;

uniform mat4 boneTransforms[MAX_BONES];
uniform bool isAnimated;

uniform mat4 parentTransform;

void main()
{
    mat4 boneTransform = boneTransforms[bonesID[0]] * weights[0];
    for (int i = 1; i < MAX_WEIGHTS; ++i)
        boneTransform += boneTransforms[bonesID[i]] * weights[i];
    mat4 transformModelMat = modelsMatrix * parentTransform * boneTransform;
    gl_Position = lightSpaceMatrix * transformModelMat * vec4(aPos, 1.0);
}