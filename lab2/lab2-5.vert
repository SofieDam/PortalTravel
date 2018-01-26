#version 150

in  vec3 in_Position;
in vec3 in_Normal;
uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
out vec3 ex_Normal;
out vec3 ex_Surface;

void main(void)
{

    ex_Normal = inverse(transpose(mat3(camMatrix * mdlMatrix))) * in_Normal; // Phong, normal transformation
    ex_Surface = vec3(camMatrix * mdlMatrix * vec4(in_Position, 1.0));

	gl_Position = projectionMatrix * camMatrix * mdlMatrix * vec4(in_Position, 1.0);
}
