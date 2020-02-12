#version 430 core

in vec3 normal;

out vec4 FragColor;

uniform vec3 lightPosition;
uniform vec3 cubeColor;

void main()
{
    // FragColor = vec4(color.r, color.g + greenValue, color.b, 1.0f);
    FragColor = vec4(cubeColor, 1.0);
}
