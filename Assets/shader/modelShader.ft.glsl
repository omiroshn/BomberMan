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
    vec3 Normal;
} fs_in;

vec3 GetNormal(vec2 texCoords)
{
    vec3 normal = texture(texture_normal1, texCoords).rgb;
    return normalize(normal * 2.0 - 1.0);
}

vec3 GetLightDir()
{
    return normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = GetNormal(fs_in.TexCoords);
    vec3 lightDir = GetLightDir();
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.001);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{
    vec3 objectcolor = texture(texture_diffuse1, fs_in.TexCoords).rgb;
    // vec3 normal = normalize(fs_in.Normal);//GetNormal(fs_in.TexCoords);
    vec3 normal = GetNormal(fs_in.TexCoords);
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.3 * objectcolor;
    // diffuse
    // vec3 lightDir = normalize(lightPos - fs_in.FragPos);//GetLightDir();
    vec3 lightDir = GetLightDir();
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    //shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);                      

    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * objectcolor;    
    // vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    // vec3 lightDir = GetLightDir();
    FragColor = vec4(lighting, 1.0);
}
