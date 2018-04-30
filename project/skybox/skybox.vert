#version 150

in  vec3 inPosition;
in  vec2 inTexCoord;
out vec2 texCoord;

uniform mat4 projMatrix;
uniform mat4 camMatrix;
uniform mat4 identityMatrix;
//uniform mat4 modelMatrix;

void main(void)
{
	texCoord = inTexCoord;

	//gl_Position = projMatrix * camMatrix * vec4(inPosition, 1.0);
	//gl_Position = projMatrix * camMatrix * identityMatrix * modelMatrix * vec4(inPosition, 1.0);
	gl_Position = projMatrix * camMatrix * identityMatrix * vec4(inPosition, 1.0);
}
