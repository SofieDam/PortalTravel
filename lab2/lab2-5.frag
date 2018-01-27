#version 150

in vec3 ex_Normal; // Phong
in vec3 ex_Surface; // Phong (specular)
out vec4 out_Color;

void main(void)
{
    // lab2-5

    const vec3 light = vec3(0.58, 0.58, 0.58);
    float diffuse, specular, shade;

    // Diffuse: Incoming light produces same intensity in all directions.
    diffuse = dot(normalize(ex_Normal), light);
    diffuse = max(0.0, diffuse); // No negative light

    // Specular: Incoming light produces higher intensity around the mirroring angle.
    vec3 reflectedLightDirection = reflect(-light, normalize(ex_Normal));
    vec3 eyeDirection = normalize(-ex_Surface); // View direction
    specular = dot(reflectedLightDirection, eyeDirection);
    if (specular > 0.0)
        specular = 1.0 * pow(specular, 150.0); // specularStrength, pow() return the value of the first parameter raised to the power of the second
    specular = max(specular, 0.0);  // specularStrength
    shade = 0.7*diffuse + 1.0*specular;

    out_Color = vec4(shade, shade, shade, 1.0);
}