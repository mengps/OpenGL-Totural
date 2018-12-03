#version 330 core

in WorldVertex
{
	vec3 normal;
	vec3 fragPos;
	vec2 texCoord;
};

struct Material
{
	sampler2D diffuse0;
	sampler2D specular0;
	float shininess;
};

out vec4 fragColor;
uniform Material material;

void main()
{
	fragColor = texture2D(material.diffuse0, texCoord);
}