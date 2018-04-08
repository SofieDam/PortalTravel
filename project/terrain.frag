#version 150

out vec4 outColor;
in vec2 texCoord;
uniform sampler2D tex;
uniform vec3 color;
in vec3 ex_Normal;
in vec3 ex_Surface;


void main(void)
{

	const vec3 light = vec3(0.6, 0.6, 0.6);
	float diffuse, specular, shade;

	// Diffuse
	diffuse = dot(normalize(ex_Normal), light);
	diffuse = max(0.0, diffuse); // No negative light

	// Specular
	vec3 r = reflect(-light, normalize(ex_Normal));
	vec3 v = normalize(-ex_Surface); // View direction
	specular = dot(r, v);
	if (specular > 0.0)
		specular = 1.0 * pow(specular, 5.0);
	specular = max(specular, 0.0);

	shade = 1.0*diffuse + 1.0*specular;

    //outColor = vec4(shade, shade, shade, 1.0);

	//outColor = vec4(shade*color, 1.0);

    //outColor = vec4(color, 1.0);

	//outColor = vec4(0.8, 0.2, 0.8, 1.0);
	//outColor = texture(tex, texCoord);
	outColor = vec4(shade, shade, shade, 1.0) * texture(tex, texCoord);
}
