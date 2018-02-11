#version 150

in  vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

uniform mat4 projMatrix;
uniform mat4 mdlMatrix;

out vec3 ex_Normal;
out vec3 ex_Surface;
out vec2 texCoord;

void main(void)
{
	texCoord = inTexCoord;

    ex_Normal = normalize(inverse(transpose(mat3(mdlMatrix))) * inNormal); // Phong, normal transformation
    ex_Surface = vec3(mdlMatrix * vec4(inPosition, 1.0));

	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
