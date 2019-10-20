#version 410 core
in float time;
in vec2 TexCoords;

out vec4 color;

uniform sampler2D image;

void main()
{
	vec4 texColor;
	texColor = texture(image, TexCoords);
	texColor.a = time;
    color = texColor;
}
