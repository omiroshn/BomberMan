#version 330 core

in vec2 UV;

out vec4 color;

uniform float power = 1;

void main()
{
	vec2 centeredUV = UV * .5f - .5f;
	float reversed = max(.5f - length(centeredUV), 0);
	float alpha = pow(reversed, power);
	color = vec4(0,1,0,alpha);
}
