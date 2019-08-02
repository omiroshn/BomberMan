#version 330 core
out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float shininess = 1;
uniform float glossiness = 0.01;
in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec4 FragPosLightSpace;
} vs_out;

vec3 GetNormal()
{
    vec3 normal = texture(texture_normal1, vs_out.TexCoords).rgb;
    return normalize(normal * 2.0 - 1.0);
}

vec3 GetLightDir()
{
    return normalize(vs_out.TangentLightPos - vs_out.TangentFragPos);;
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    vec3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w) * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.001);
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
    vec3 ambient = lightAmbient * texture(texture_diffuse1, vs_out.TexCoords).rgb;
    // diffuse
    vec3 normal = GetNormal();
    vec3 lightDir = GetLightDir();
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * diff * texture(texture_diffuse1, vs_out.TexCoords).rgb;
    //shadow
    float shadow = ShadowCalculation(vs_out.FragPosLightSpace, normal, lightDir);
    // specular
    vec3 viewDir = normalize(viewPos - vs_out.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * glossiness * lightSpecular;

    vec3 result = min(ambient + (diffuse + specular) * (1.0 - shadow), 1.0);
    FragColor = vec4(result, 1.0);
}
