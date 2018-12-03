#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal0;
layout (location = 2) in vec2 texCoord0;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out WorldVertex
{
	vec3 normal;
	vec3 fragPos;
	vec2 texCoord;
};	

uniform mat3 normalMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	normal = normalMatrix * normal0;
	fragPos = vec3(model * vec4(position, 1.0f));
	texCoord = texCoord0;	
}