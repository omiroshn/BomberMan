#version 410 core
out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2DShadow shadowMap;

uniform vec3 viewPos;

uniform float shininess;
uniform float glossiness;
in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightDir;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec4 FragPosLightSpace;
} fs_in;

vec3 GetNormal()
{
    vec3 normal = texture(texture_normal1, fs_in.TexCoords).rgb;
    return normalize(normal * 2.0 - 1.0);
}

vec3 GetLightDir()
{
    return fs_in.TangentLightDir;
}

#define SAMPLES 3
#define OFFSET_SIZE 0.9

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w) * 0.5 + 0.5;
    if (projCoords.z > 1.f)
        return 0.0;
    float shadow = 0.0;
    for (int i = -SAMPLES; i <= SAMPLES; i++)
    for (int j = -SAMPLES; j <= SAMPLES; j++)
    {
        vec3 sampleCoords = projCoords;
        sampleCoords += vec3(vec2(i, j) * (1. / textureSize(shadowMap, 0)) * OFFSET_SIZE, 0.f);
        shadow += texture(shadowMap, sampleCoords);
    }
    return shadow / float((SAMPLES * 2 + 1) * (SAMPLES * 2 + 1));
}

void main()
{
    vec3 lightAmbient = vec3(.6f);
    vec3 lightDiffuse = vec3(1.f);
    vec3 lightSpecular = vec3(1.f);

    // ambient
    vec3 ambient = lightAmbient * texture(texture_diffuse1, fs_in.TexCoords).rgb;
    // diffuse
    vec3 normal = GetNormal();
    vec3 lightDir = GetLightDir();
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * diff * texture(texture_diffuse1, fs_in.TexCoords).rgb;
    //shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * glossiness * lightSpecular;

    vec3 result = ambient + (diffuse + specular) * (1.0 - shadow);
    result = pow(result, vec3(1.16f));
    FragColor = vec4(result, 1.0);
}
