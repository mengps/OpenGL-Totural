#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord0;
layout (location = 2) in vec3 normal0;

out vec2 texCoord;
out WorldVertexData
{
	vec3 normal;
	vec3 fragPos;
};	

uniform mat3 normalMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	texCoord = texCoord0;
	normal = normalMatrix * normal0;
	fragPos = vec3(model * vec4(position, 1.0f));
}