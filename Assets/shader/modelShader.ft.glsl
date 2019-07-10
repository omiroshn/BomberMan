#version 330 core
out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
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
    return normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);;
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
    vec3 objectcolor = texture(texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 normal = GetNormal();
    vec3 lightColor = vec3(0.4);
    vec3 lightDir = GetLightDir();
    // ambient
    vec3 ambient = 0.3 * objectcolor;
    // diffuse
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor; 
    //shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, normal, lightDir);                      
    vec3 lighting = (ambient + (1.0 - shadow) * diffuse) * objectcolor;    
    FragColor = vec4(lighting, 1.0);
}
