#version 330 core

const int MAX_JOINTS = 50;//max joints allowed in a skeleton
const int MAX_WEIGHTS = 3;//max number of joints that can affect a vertex

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 modelsMatrix;

layout(location = 7) in ivec3 jointIndices;
layout(location = 8) in vec3 weights;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 view;
uniform mat4 projection;

uniform mat4 jointTransforms[MAX_JOINTS];
uniform bool isAnimated;

void main()
{
    mat4 transformModelMat;

    if (isAnimated)
    {
        mat4 jointTransform = jointTransforms[jointIndices[0]] * weights[0];
        for (int i = 1; i < MAX_WEIGHTS; ++i)
        {
            jointTransform += jointTransforms[jointIndices[i]] * weights[i];
        }
        transformModelMat = modelsMatrix * jointTransform;
    }
    else
    {
        transformModelMat = modelsMatrix;
    }

    gl_Position = projection * view * transformModelMat * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
    Normal = mat3(transpose(inverse(transformModelMat))) * aNormal;
    FragPos = vec3(transformModelMat * vec4(aPos, 1.0));
}