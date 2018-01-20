#version 150

in  vec3 in_Position;
in vec3 in_Triangle_Color;
out vec3 ex_Triangle_Color;

void main(void)
{
    ex_Triangle_Color = in_Triangle_Color;
	gl_Position = vec4(in_Position, 1.0);
}
