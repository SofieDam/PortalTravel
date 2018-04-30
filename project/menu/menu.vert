#version 150

in  vec3 inPosition;
in  vec2 inTexCoord;
out vec2 texCoord;
uniform mat4 modelMatrix;

void main(void)
{
	texCoord = inTexCoord;

	gl_Position = modelMatrix * vec4(inPosition, 1.0);
}
