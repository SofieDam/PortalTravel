#version 150

in vec3 ex_Normal; // Phong
in vec3 ex_Surface; // Phong (specular)

uniform sampler2D texUnit;

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];
uniform vec3 eyePosition;

out vec4 out_Color;

void main(void)
{
    vec3 shade = vec3(0,0,0);
    vec3 diffuse = vec3(0,0,0);
    vec3 specular = vec3(0,0,0);

    // Diffuse
    float diffuseStrength;
    for(int i = 0; i < 4; i++)
    {
    		diffuseStrength = dot(ex_Normal, lightSourcesDirPosArr[i]);
    		diffuseStrength = max(0.0, diffuseStrength); // No negative light
    		diffuse += diffuseStrength * lightSourcesColorArr[i];
    }

    // Specular
    float specularStrength;
    vec3 reflectedLightDirection;
    vec3 eyeDirection;
    for(int i = 0; i < 4; i++)
    {
        if (isDirectional[i])   // Directional vector
        {
            reflectedLightDirection = reflect(lightSourcesDirPosArr[i], ex_Normal);
        }
        else    // Positional vector
        {
            reflectedLightDirection = reflect(normalize(ex_Surface-lightSourcesDirPosArr[i]), ex_Normal);
        }

        eyeDirection = normalize(eyePosition-ex_Surface); // View direction
        specularStrength = dot(reflectedLightDirection, eyeDirection);

        if (specularStrength > 0.0)
        {
            specularStrength = 1.0 * pow(specularStrength, specularExponent[i]); // pow() return the value of the first parameter raised to the power of the second
        }
        specularStrength = max(specularStrength, 0.0);
        specular += specularStrength * lightSourcesColorArr[i];
    }

    shade = diffuse + specular;
    out_Color = vec4(shade, 1.0);
}