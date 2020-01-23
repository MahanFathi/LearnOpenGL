#version 430 core

in vec3 vertexPosition;
in vec3 vertexColor;
in vec2 vertexTexture;

out vec3 color;
out vec2 txtr;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    color = vertexColor;
    txtr = vertexTexture;
    gl_Position = projection * view * model * transform * vec4(vertexPosition, 1.0);
}
