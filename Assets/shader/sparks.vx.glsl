#version 410 core
layout (location = 0) in vec4 vertex;

out float outX;
out float outY;

uniform float offsetX;
uniform float offsetY;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    float time = vertex.w;
    gl_Position = projection * view * vec4(vertex.xyz, 1.0);
    gl_PointSize = (800 + 500 * time) / gl_Position.z / 2;
    outX = offsetX;
    outY = offsetY;
}
