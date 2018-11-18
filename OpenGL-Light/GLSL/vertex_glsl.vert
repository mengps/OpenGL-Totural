#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color0;
layout (location = 2) in vec3 normal0;

out vec3 color;
out vec4 fragPos;	
out vec3 normal;

uniform mat3 normalMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	color = color0;
	fragPos = model * vec4(position, 1.0f);
	normal = normalMatrix * normal0;
}