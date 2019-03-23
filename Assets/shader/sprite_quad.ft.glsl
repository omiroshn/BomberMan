#version 330 core
in vec4 colorBuf;
in vec2 TexCoords;

out vec4 color;

uniform sampler2D image;

void main()
{
	vec4 texColor = texture(image, TexCoords) * colorBuf;
	if (texColor.a < 0.02f)
		discard;
    color = texColor;
}
