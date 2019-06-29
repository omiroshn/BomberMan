#version 330 core
in float 	vtime;
out vec4 	color;

uniform sampler2D image;

void main()
{
    vec4 tex = texture(image, gl_PointCoord) / vtime;
    if (tex.a < 0.01)
        discard;
    color =  vec4(tex.rgb * tex.a, 1);
}
