#version 330 core
in float time;
in vec2 TexCoords;

out vec4 color;

uniform sampler2D image;

void main()
{
	vec4 texColor = texture(image, TexCoords);
	if (texColor.a < 0.05f)
		discard;
    color = texColor;
}
