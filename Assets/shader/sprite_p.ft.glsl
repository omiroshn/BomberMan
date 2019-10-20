#version 410 core
in float 	vtime;
out vec4 	color;

uniform sampler2D image;

void main()
{
    vec4 tex = texture(image, gl_PointCoord) / vtime;
    if (tex.a < 0.01)
        discard;
    color = tex;
}
