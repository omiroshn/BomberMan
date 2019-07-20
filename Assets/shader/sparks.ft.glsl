// #version 330 core

// in vec2 v_TexCoord;
// out vec4 color;

// uniform vec4 u_Color;
// uniform sampler2D u_Texture;

// void main()
// {
//     color = texture(u_Texture, v_TexCoord);
// }

#version 330 core
in float 	vtime;
out vec4 	color;

uniform sampler2D image;

void main()
{
    vec4 tex = texture(image, gl_PointCoord) / vtime;
    // if (tex.a < 0.01)
    //     discard;
    color = tex;
}
