#version 410 core

const int MAX_BONES = 50;
const int MAX_WEIGHTS = 3;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 modelsMatrix;

layout (location = 7) in ivec3 bonesID;
layout (location = 8) in vec3 weights;

layout (location = 9) in vec3 aTangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightDir;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 view;
uniform mat4 projection;

uniform mat4 boneTransforms[MAX_BONES];

uniform vec3 lightDir;
uniform vec3 viewPos;
uniform mat4 lightSpaceMatrix;

uniform mat4 parentTransform;

void main()
{
    mat4 boneTransform = boneTransforms[bonesID[0]] * weights[0];
    for (int i = 1; i < MAX_WEIGHTS; ++i)
    {
        boneTransform += boneTransforms[bonesID[i]] * weights[i];
    }
    mat4 transformModelMat = modelsMatrix * parentTransform * boneTransform;
    gl_Position = projection * view * transformModelMat * vec4(aPos, 1.0);
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPos = vec3(transformModelMat * vec4(aPos, 1.0));

    mat3 normalMatrix = transpose(inverse(mat3(transformModelMat)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));

    vs_out.TangentLightDir = TBN * lightDir;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
}