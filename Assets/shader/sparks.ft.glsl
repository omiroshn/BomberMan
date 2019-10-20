#version 410 core

out vec4 color;

in float outX;
in float outY;

float scale = 1.0 / 8.0;

uniform sampler2D image;

void main()
{
    color = texture(image, gl_PointCoord * scale + vec2(outX, outY));
}
