#version 150

in vec3 ex_Normal;
in vec3 ex_Surface;
uniform vec3 color;
out vec4 outColor;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float diffuse, specular, shade;
	
	// Diffuse
	diffuse = dot(normalize(ex_Normal), light);
	diffuse = max(0.0, diffuse); // No negative light
	
	// Specular
	vec3 r = reflect(-light, normalize(ex_Normal));
	vec3 v = normalize(-ex_Surface); // View direction
	specular = dot(r, v);
	if (specular > 0.0)
		specular = 1.0 * pow(specular, 150.0);
	specular = max(specular, 0.0);

	shade = 0.7*diffuse + 1.0*specular;

	outColor = vec4(shade*color, 1.0);
}
