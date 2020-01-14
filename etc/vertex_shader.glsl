#version 430 core
in vec3 vertexPosition;
in vec3 vertexColor;
out vec3 color;

void main()
{
    color = vertexColor;
    gl_Position = vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1.0);
}
