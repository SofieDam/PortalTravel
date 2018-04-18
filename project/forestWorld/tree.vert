#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;

// NY
uniform mat4 projMatrix;
uniform mat4 camMatrix;
uniform mat4 identityMatrix;
uniform mat4 modelMatrix;

out vec3 ex_Normal;
out vec3 ex_Surface;



void main(void)
{
	texCoord = inTexCoord;

	//vec3 position = inPosition * normalize(inPosition);
	vec3 position = inPosition;

    ex_Normal = inverse(transpose(mat3(camMatrix * identityMatrix * modelMatrix))) * inNormal; // Phong, normal transformation
    ex_Surface = vec3(camMatrix * identityMatrix * modelMatrix * vec4(position, 1.0));

	gl_Position = projMatrix * camMatrix * identityMatrix * modelMatrix * vec4(position, 1.0);
}