#version 410 core
layout (location = 0) in vec4 iVertex;
layout (location = 1) in vec2 iUV;

out vec2 UV;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * iVertex;
    UV = iUV;
}
