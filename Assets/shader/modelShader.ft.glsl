#version 330 core
out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;
uniform bool hasNormalMap;

uniform vec3 lightPos;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 Normal;
} fs_in;

vec3 GetNormal(vec2 texCoords);
vec3 GetLightDir();

void main()
{
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
    vec3 objectcolor = texture(texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 lightDir = GetLightDir();
    vec3 normal = GetNormal(fs_in.TexCoords);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 ambient = 0.1 * objectcolor;
    vec3 diffuse = diff * objectcolor;
    FragColor = vec4(ambient + diffuse, 1.0);
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