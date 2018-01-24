#version 150

in vec3 normal;
out vec4 out_Color;

void main(void)
{
	const vec3 light = vec3(0.6, 0.6, 0.6);
	float shade = dot(normalize(normal), light);
	out_Color = vec4(shade, shade, shade, 1.0);
}