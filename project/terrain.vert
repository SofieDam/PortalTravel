#version 150

//in  vec3 inPosition;
//in  vec3 inNormal;
//in vec2 inTexCoord;
//out vec2 texCoord;

// NY
uniform mat4 projMatrix;
uniform mat4 camMatrix;
//uniform mat4 mdlMatrix;

void main(void)
{
	gl_Position = projMatrix * camMatrix;
	//gl_Position = projMatrix * camMatrix * vec4(inPosition, 1.0);
}
