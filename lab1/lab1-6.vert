#version 150

in  vec3 in_Position;
in vec3 in_Normal;
in vec3 in_Triangle_Color;
uniform mat4 myMatrixX;
uniform mat4 myMatrixY;
uniform mat4 myMatrixZ;
out vec3 normal;

void main(void)
{
   normal = in_Normal;

	gl_Position = myMatrixX * myMatrixY * myMatrixZ * vec4(in_Position, 1.0);
}
