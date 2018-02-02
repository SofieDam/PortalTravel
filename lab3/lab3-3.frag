#version 150

in vec2 ex_Tex_Coord;
uniform sampler2D texUnit;
out vec4 outColor;

void main(void)
{
    outColor = texture(texUnit, ex_Tex_Coord);
}