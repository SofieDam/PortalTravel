#version 150

in vec3 texcoord;
out vec4 out_Color;

void main(void)
{
	float a = sin(texcoord.s*30)/2+0.5;
    float b = sin(texcoord.t*30)/2+0.5;
	out_Color = vec4(a, b, 1.0, 0.0);
}