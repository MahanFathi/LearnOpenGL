#version 430 core

in vec3 vertexPosition;
in vec3 vertexNormal;

out vec3 normal;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    normal = vertexNormal;
    gl_Position = projection * view * model * transform * vec4(vertexPosition, 1.0);
}
