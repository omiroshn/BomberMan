// #version 330 core

// layout(location = 0) in vec4 position;
// layout(location = 1) in vec2 texCoord;

// out vec2 v_TexCoord;

// uniform mat4 u_MVP;
// uniform float offsetX;
// uniform float offsetY;

// float scale = 1.0 / 8.0;

// void main()
// {
//     v_TexCoord = texCoord * scale + vec2(offsetX, offsetY);
//     gl_Position - u_MVP * vec3(position.xy, 0.0, 1.0);
// }

#version 330 core
layout (location = 0) in vec4 vertex;
// layout (location = 1) in vec2 texCoord;

// out float vtime;
// out vec2 v_TexCoord;

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
    gl_PointSize = (800 + 1000 * time) / gl_Position.z / 2;
    // v_TexCoord = texCoord * scale + offset;
	// vtime = time * 25;
    outX = offsetX;
    outY = offsetY;
}
