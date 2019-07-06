#version 330 core
layout (location = 0) in vec4 vertex;
layout (location = 2) in mat4 model;

out float vtime;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    float time = vertex.w;
    gl_Position = projection * view * model * vec4(vertex.xyz, 1.0);
    gl_PointSize = (800 + 1000 * time) / gl_Position.z;
	vtime = time * 25;
}
