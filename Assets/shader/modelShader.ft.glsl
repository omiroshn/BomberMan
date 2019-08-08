#version 330 core
out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D shadowMap;

uniform vec3 viewPos;

uniform float shininess;
uniform float glossiness;
in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 VertNormal;
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

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    vec3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w) * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float bias = max(0.000004 * (1.0 - dot(normal, lightDir)), 0.0001);
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

void main()
{
    vec3 lightAmbient = vec3(.2f);
    vec3 lightDiffuse = vec3(.5f);
    vec3 lightSpecular = vec3(1.f);

    // ambient
    vec3 ambient = lightAmbient * texture(texture_diffuse1, fs_in.TexCoords).rgb;
    // diffuse
    vec3 normal = GetNormal();
    vec3 lightDir = GetLightDir();
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * diff * texture(texture_diffuse1, fs_in.TexCoords).rgb;
    //shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, fs_in.VertNormal, lightDir);
    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * glossiness * lightSpecular;

    vec3 result = min(ambient + (diffuse + specular) * (1.0 - shadow), 1.0);
    FragColor = vec4(result, 1.0);
}
