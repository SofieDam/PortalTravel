#version 150

in  vec3 in_Position;
in vec2 in_Texture;
in vec3 in_Normal;
uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
out vec2 texCoord;

void main(void)
{
    texCoord = in_Texture;

	gl_Position = projectionMatrix * camMatrix * mdlMatrix * vec4(in_Position, 1.0);
}
