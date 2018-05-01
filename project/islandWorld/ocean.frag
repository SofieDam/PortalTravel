#version 150

out vec4 outColor;
uniform vec3 color;
in vec3 ex_Normal;
in vec3 ex_Surface;


void main(void)
{
    const vec3 color = vec3(0.0, 0.0, 1.0);
    float transparency = 0.3;

    outColor = vec4(color, transparency);
}
