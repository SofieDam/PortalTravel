#version 150

in  vec3 in_Position;
in vec3 in_Texture;
uniform mat4 myMatrixX;
uniform mat4 myMatrixY;
uniform mat4 myMatrixZ;
out vec3 texcoord;

void main(void)
{
    texcoord = in_Texture;

	gl_Position = myMatrixX * myMatrixY * myMatrixZ * vec4(in_Position, 1.0);
}
