#version 150

in vec2 ex_Tex_Coord;
in vec3 ex_Normal; // Phong
uniform sampler2D texUnit;
out vec4 outColor;

void main(void)
{
    outColor = texture(texUnit, ex_Tex_Coord);
}