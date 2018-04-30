#version 150

out vec4 outColor;
//in vec2 texCoord;
//uniform sampler2D tex;
uniform vec3 color;
in vec3 ex_Normal;
in vec3 ex_Surface;


void main(void)
{

/*
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
		specular = pow(specular, 1000.0);
	specular = max(specular, 0.0);

	shade = 1.0*diffuse + 1.0*specular;
*/

/*
    const vec3 light = vec3(0.6, 0.6, 0.6);
    float diffuse, specular, shade;

    // Fake Phong
    const float step = 1.0/40.0; // Should match size of texture
    float bt = texture(tex, texCoord).r - texture(tex, texCoord + vec2(0.0, step)).r;
    float bs = texture(tex, texCoord).r - texture(tex, texCoord + vec2(step, 0.0)).r;
    // fake it
    vec3 ps, pt;
    ps = 2.0 * cross(ex_Normal, normalize(vec3(1.0, 0.0, 0.0)));
    pt = 2.0 * cross(ex_Normal, ps);
    vec3 n = ex_Normal + ps * bs + pt * bt;


    // Diffuse
    diffuse = dot(normalize(n), light);
    diffuse = max(0.0, diffuse); // No negative light


    // Specular
    vec3 r = reflect(-light, normalize(n));
    vec3 v = normalize(-ex_Surface); // View direction
    specular = dot(r, v);
    if (specular > 0.0)
        specular = 1.0 * pow(specular, 500.0);
    specular = max(specular, 0.0);
    shade = 1.0*diffuse + 1.0*specular;
*/


    // Color
    const vec3 color = vec3(0.0, 0.0, 1.0);
    float transparency = 0.3;

    outColor = vec4(color, transparency);


    //outColor = vec4(shade*color, transparency);

    //outColor = texture(tex, texCoord);
    //outColor = shade * vec4(color, transparency);


}
