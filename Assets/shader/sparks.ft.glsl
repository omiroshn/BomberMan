// #version 330 core

// in vec2 v_TexCoord;
// out vec4 color;

// uniform vec4 u_Color;
// uniform sampler2D image;

// void main()
// {
//     color = texture(image, v_TexCoord);
// }

#version 330 core

// in vec2  v_TexCoord;
out vec4 color;

in float outX;
in float outY;

float scale = 1.0 / 8.0;

uniform sampler2D image;

void main()
{
    color = texture(image, gl_PointCoord * scale + vec2(outX, outY));
    // color = texture(image, v_TexCoord);
}
