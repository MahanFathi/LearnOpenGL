#version 430 core

in vec3 color;
in vec2 txtr;

out vec4 FragColor;

uniform float greenValue;
uniform sampler2D textureSampler;

void main()
{
    // FragColor = vec4(color.r, color.g + greenValue, color.b, 1.0f);
    FragColor = texture(textureSampler, txtr) +
        vec4(color.r, color.g + greenValue, color.b, 1.0);
}
