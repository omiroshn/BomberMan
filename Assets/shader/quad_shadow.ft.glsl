#version 410 core

in vec2 UV;

out vec4 color;

uniform float power = 1;
uniform float radius = 1;

void main()
{
	vec2 centeredUV = (UV - .5f);
	float alpha = max(.5f - length(centeredUV), 0);
	if (alpha < 0.001)
		discard;
	color = vec4(0,0,0,alpha);
}
