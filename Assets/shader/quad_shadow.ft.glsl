#version 330 core

in vec2 UV;

out vec4 color;

uniform float power = 1;

void main()
{
	vec2 centeredUV = UV * .5f - .5f;
	float reversed = max(.5f - length(centeredUV), 0);
	float alpha = pow(reversed, power);
	if (alpha < 0.01)
		discard;
	color = vec4(0,0,0, alpha);
}
