#version 330 core
layout (location = 0) in vec4 vertex;
layout (location = 1) in vec4 color;
layout (location = 2) in mat4 model;

out vec4 colorBuf;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * model * vec4(vertex.xyz, 1.0);
	colorBuf = color;
}
