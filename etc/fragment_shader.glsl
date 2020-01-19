#version 430 core

in vec3 color;
in vec2 txtr;

out vec4 FragColor;

uniform float greenValue;
uniform sampler2D textureSampler0;
uniform sampler2D textureSampler1;

void main()
{
    // FragColor = vec4(color.r, color.g + greenValue, color.b, 1.0f);
    FragColor = mix(texture(textureSampler0, txtr), texture(textureSampler1, txtr), 0.2) +
        vec4(color.r, color.g + greenValue, color.b, 1.0);
}
