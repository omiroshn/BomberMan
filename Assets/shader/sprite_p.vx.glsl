#version 330 core
layout (location = 0) in vec4 vertex;
layout (location = 2) in mat4 model;

out vec4 colorBuf;

uniform mat4 projection;
uniform mat4 view;

#define	COLOR_MAX	vec4(1.0f, 0.4f, 0.0f, 0.51f)
#define	COLOR_MIN	vec4(0.5f, 0.2f, 0.0f, 0.01f)
#define	LIVE_TIME   2.0f

void main()
{
    gl_Position = projection * view * model * vec4(vertex.xyz, 1.0);
	colorBuf = mix(COLOR_MAX, COLOR_MIN, vertex.w / LIVE_TIME);
}
