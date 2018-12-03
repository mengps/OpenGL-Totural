#version 330 core

uniform vec3 lightColor;
out vec4 fragColor;

void main(void)
{
	fragColor = vec4(lightColor, 1.0f);
}