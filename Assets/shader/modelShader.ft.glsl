#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;

uniform vec3 lightPos;
void main()
{
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);

    //diffuse light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * lightColor;

    //ambient light
    float ambientStrength = 0.1;
    vec4 ambient = ambientStrength * lightColor;

    FragColor = (ambient + diffuse) * texture(texture_diffuse1, TexCoords);
}