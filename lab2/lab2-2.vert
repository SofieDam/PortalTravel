#version 150

in  vec3 in_Position;
in vec2 in_Texture;
in vec3 in_Normal;
uniform mat4 myMatrixX;
uniform mat4 myMatrixY;
uniform mat4 myMatrixZ;
out vec2 texCoord;

void main(void)
{
    texCoord = in_Texture;

	gl_Position = myMatrixX * myMatrixY * myMatrixZ * vec4(in_Position, 1.0);
}
