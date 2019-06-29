#version 330 core
out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform bool hasNormalMap;
uniform vec3 lightPos;

float specularStrength = 0.5;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
    vec3 Normal;
} fs_in;

void main()
{
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
    vec3  norm;
    if (hasNormalMap)
    {
        norm = texture(texture_normal1, fs_in.TexCoords).rgb;
        norm = normalize(norm * 2.0 - 1.0);
        norm = normalize(fs_in.TBN * norm);
    }
    else
        norm = normalize(fs_in.Normal);

    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * lightColor;

    //ambient light
    float ambientStrength = 0.1;
    vec4 ambient = ambientStrength * lightColor;

    FragColor = (ambient + diffuse) * texture(texture_diffuse1, fs_in.TexCoords);
}