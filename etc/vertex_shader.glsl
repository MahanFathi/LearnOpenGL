#version 430 core
in vec3 vertex_position;
layout (location = 1) in vec3 dummy;
layout (location = 2) in vec3 vertex_color;
out vec3 color;

void main()
{
    color = vertex_color;
    gl_Position = vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);
}
