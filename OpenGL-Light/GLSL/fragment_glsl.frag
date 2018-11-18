#version 330 core

in vec3 color;
in vec4 fragPos;
in vec3 normal;
out vec4 fragColor;

uniform vec3 viewPosition;
uniform vec3 lightColor;
uniform vec3 lightPosition;

void main(void)
{
	float strength = 0.2f;
	vec3 ambient = lightColor * strength;

	vec3 lightDir = normalize(lightPosition - vec3(fragPos));
	float diff = max(dot(normalize(normal), lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPosition - vec3(fragPos));
	vec3 reflectDir = reflect(-lightDir, normalize(normal));
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), 32);
	vec3 specular = 0.8f * spec * lightColor;

	vec3 resultColor = (diffuse + ambient + specular) * color;
	fragColor = vec4(resultColor, 1.0f);
}