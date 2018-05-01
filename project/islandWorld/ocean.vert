#version 150

in vec3 inPosition;
in vec3 inNormal;

uniform mat4 projMatrix;
uniform mat4 camMatrix;
uniform mat4 identityMatrix;
uniform mat4 modelMatrix;
uniform float currentTime;

out vec3 ex_Normal;
out vec3 ex_Surface;



void main(void)
{
	vec3 position = inPosition;

    // Create "waves"
    float amplitude = 0.001;       // metres
    float period  = 5;             // seconds
    float twoPi = 6.28318530718;   // 2*pi
    float time = (sin(currentTime / period * twoPi) + 1.0) / 2.0;
    position += inNormal * time * amplitude;

    ex_Normal = inverse(transpose(mat3(camMatrix * identityMatrix * modelMatrix))) * inNormal;
    ex_Surface = vec3(camMatrix * identityMatrix * modelMatrix * vec4(position, 1.0));

    gl_Position = projMatrix * camMatrix * identityMatrix * modelMatrix * vec4(position, 1.0);
}
