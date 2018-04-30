#version 150

out vec4 outColor;
in vec2 texCoord;
uniform sampler2D tex;
uniform int mouseOver;

void main(void)
{
    float transparency = 0.8;
    vec4 mouseOverColor = vec4(1.0, 1.0, 1.0, transparency);  // White

    if ((mouseOver == 1) || (mouseOver == 2)) {
            outColor = texture(tex, texCoord);
    }
    else {
            outColor = mouseOverColor * texture(tex, texCoord);
    }
}
