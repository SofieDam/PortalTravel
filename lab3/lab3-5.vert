#version 150

in  vec3 in_Position;
in  vec2 in_Tex_Coord;

uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;

out vec2 ex_Tex_Coord;

void main(void)
{
    ex_Tex_Coord = in_Tex_Coord;

	gl_Position = projectionMatrix * camMatrix * mdlMatrix * vec4(in_Position, 1.0);
}
