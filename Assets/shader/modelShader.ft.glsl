#version 330 core
out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;
uniform bool hasNormalMap;
uniform bool hasHeightMap;

uniform vec3 lightPos;

float specularStrength = 0.5;
uniform float height_scale;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 Normal;
} fs_in;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);
vec3 GetNormal(vec2 texCoords);
vec3 GetLightDir();

void main()
{
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
    vec3 objectcolor = texture(texture_diffuse1, fs_in.TexCoords).rgb;

    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);

    vec3 lightDir = GetLightDir();

    vec2 texCoords = ParallaxMapping(fs_in.TexCoords,  viewDir);
    vec3 normal = GetNormal(texCoords);

    float diff = max(dot(lightDir, normal), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 ambient = 0.1 * objectcolor;
    vec3 diffuse = diff * objectcolor;
    vec3 specular = vec3(0.2) * spec;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    vec2 p = vec2(0);
    if (hasHeightMap)
    {
        float height = texture(texture_height1, texCoords).r;
        p = viewDir.xy / viewDir.z * (height * 0.02);
    }
    return texCoords - p;
}

vec3 GetNormal(vec2 texCoords)
{
    vec3 normal;
    if (hasNormalMap)
    {
        normal = texture(texture_normal1, texCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);
    }
    else
    normal = normalize(fs_in.Normal);
    return normal;
}

vec3 GetLightDir()
{
    vec3 lightDir;
    if (hasNormalMap)
    {
        lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    }
    else
        lightDir = normalize(lightPos - fs_in.FragPos);
    return lightDir;
}