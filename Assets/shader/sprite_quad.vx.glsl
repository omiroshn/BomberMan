#version 410 core
layout (location = 0) in vec4 vertex;
//layout (location = 1) in vec4 color;
layout (location = 2) in vec2 uv;
layout (location = 3) in mat4 model;

out vec4 colorBuf;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

#define	COLOR_MAX	vec4(1.0f, 1.0f, 1.0f, 0.71f)
#define	COLOR_MIN	vec4(0.6f, 0.6f, 0.6f, 0.01f)
#define	LIVE_TIME   2.0f

void main()
{
    gl_Position = projection * view * model * vec4(vertex.xyz, 1.0);
	colorBuf = mix(COLOR_MAX, COLOR_MIN, vertex.w / LIVE_TIME);
	TexCoords = uv;
}
