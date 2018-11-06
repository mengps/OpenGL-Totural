#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color0;
layout (location = 2) in vec2 texCoord0;

uniform mat4 mvp;
out vec3 color;
out vec2 texCoord;

void main()
{
	gl_Position = mvp * vec4(position, 1.0);
	color = color0;
	texCoord = texCoord0;
}