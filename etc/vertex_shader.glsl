#version 430 core

in vec3 vertexPosition;
in vec3 vertexColor;
in vec2 vertexTexture;

out vec3 color;
out vec2 txtr;

void main()
{
    color = vertexColor;
    txtr = vertexTexture;
    gl_Position = vec4(vertexPosition, 1.0);
}
