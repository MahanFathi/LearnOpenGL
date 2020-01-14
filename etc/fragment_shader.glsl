#version 430 core
in vec3 color;
out vec4 FragColor;

uniform float greenValue;

void main()
{
    FragColor = vec4(color.r, color.g + greenValue, color.b, 1.0f);
}
