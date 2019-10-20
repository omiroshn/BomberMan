#version 410 core
layout (location = 0) in vec4 vertex;
layout (location = 2) in vec2 uv;
layout (location = 3) in mat4 model;

out vec4 colorBuf;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

#define	COLOR_BASE	vec4(1.0f, 1.0f, 1.0f, 1.0f)

void main()
{
    gl_Position = projection * view * model * vec4(vertex.xyz, 1.0);
	colorBuf = COLOR_BASE;
	TexCoords = uv;
}
