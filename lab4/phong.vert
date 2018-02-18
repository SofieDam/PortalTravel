#version 150

in  vec3 inPosition;
in  vec3 inNormal;
out vec3 ex_Normal; // Phong
out vec3 ex_Surface; // Phong (specular)

uniform mat4 mdlMatrix;
uniform mat4 projMatrix;

void main(void)
{
    ex_Normal = inverse(transpose(mat3(mdlMatrix))) * inNormal; // Phong, normal transformation
    ex_Surface = vec3(mdlMatrix * vec4(inPosition, 1.0));

    gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
