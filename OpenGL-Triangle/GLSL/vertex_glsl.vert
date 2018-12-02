#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color0;

out vec3 color;
uniform mat4 mvp;

void main(void)
{
    gl_Position = mvp * vec4(position, 1.0f);
    color = color0;
}
