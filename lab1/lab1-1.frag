#version 150

in vec3 ex_Triangle_Color;
out vec4 out_Color;

void main(void)
{
	out_Color = vec4(ex_Triangle_Color,1.0);
}
