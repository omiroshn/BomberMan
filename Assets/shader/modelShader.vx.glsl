#version 330 core

const int MAX_BONES = 50;
const int MAX_WEIGHTS = 3;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 modelsMatrix;

layout(location = 7) in ivec3 bonesID;
layout(location = 8) in vec3 weights;

layout (location = 9) in vec3 aTangent;
layout (location = 10) in vec3 aBitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 Normal;
} vs_out;

uniform mat4 view;
uniform mat4 projection;

uniform mat4 boneTransforms[MAX_BONES];
uniform bool isAnimated;

uniform bool hasNormalMap;
uniform vec3 lightPos;

void main()
{
    mat4 transformModelMat;

    if (isAnimated)
    {
        mat4 boneTransform = boneTransforms[bonesID[0]] * weights[0];
        for (int i = 1; i < MAX_WEIGHTS; ++i)
        {
            boneTransform += boneTransforms[bonesID[i]] * weights[i];
        }
        transformModelMat = modelsMatrix * boneTransform;
    }
    else
    {
        transformModelMat = modelsMatrix;
    }


    gl_Position = projection * view * transformModelMat * vec4(aPos, 1.0);
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPos = vec3(transformModelMat * vec4(aPos, 1.0));

    if (hasNormalMap)
    {
        vec3 T   = normalize(mat3(transformModelMat) * aTangent);
        vec3 B   = normalize(mat3(transformModelMat) * aBitangent);
        vec3 N   = normalize(mat3(transformModelMat) * aNormal);
        mat3 TBN = transpose(mat3(T, B, N));

        vs_out.TangentLightPos = TBN * lightPos;
        vs_out.TangentViewPos  = TBN * lightPos;
        vs_out.TangentFragPos  = TBN * vs_out.FragPos;
    }
    else
    {
        vs_out.Normal =  mat3(transpose(inverse(transformModelMat))) * aNormal;
    }

}