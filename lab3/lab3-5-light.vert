#version 150

in  vec3 in_Position;
in vec3 in_Normal;
in  vec2 in_Tex_Coord;

uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;

out vec3 ex_Normal;
out vec3 ex_Surface;
out vec2 ex_Tex_Coord;


void main(void)
{
    //ex_Normal = inverse(transpose(mat3(camMatrix * mdlMatrix))) * in_Normal; // Phong, normal transformation
    //ex_Surface = vec3(camMatrix * mdlMatrix * vec4(in_Position, 1.0));

    // lab3-4
    ex_Normal = normalize(inverse(transpose(mat3(mdlMatrix))) * in_Normal); // Phong, normal transformation
    ex_Surface = vec3(mdlMatrix * vec4(in_Position, 1.0));

    ex_Tex_Coord = in_Tex_Coord;

	gl_Position = projectionMatrix * camMatrix * mdlMatrix * vec4(in_Position, 1.0);
}
