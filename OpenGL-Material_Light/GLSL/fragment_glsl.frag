#version 330 core

in vec2 texCoord;
in WorldVertexData
{
	vec3 normal;
	vec3 fragPos;
};

out vec4 fragColor;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 direction;

	float cutOff;
	float outerCutOff;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform vec3 viewPos;
uniform Light lights[6];
uniform Material material;

vec3 directional_lighting(Light light, Material material, vec3 viewDir, vec3 normal, vec2 texCoord);
vec3 point_lighting(Light light, Material material, vec3 viewDir, vec3 fragPos, vec3 normal, vec2 texCoord);
vec3 spot_lighting(Light light, Material material, vec3 viewDir, vec3 fragPos, vec3 normal, vec2 texCoord);

void main()
{
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 norm = normalize(normal);

	vec3 resultColor = directional_lighting(lights[1], material, viewDir, norm, texCoord);
	for (int i = 2; i < 6; i++)
	{
		resultColor += point_lighting(lights[i], material, viewDir, fragPos, norm, texCoord);
	}
	
	resultColor += spot_lighting(lights[0], material, viewDir, fragPos, norm, texCoord);
	
	fragColor = vec4(resultColor, 1.0f);
}

vec3 directional_lighting(Light light, Material material, vec3 viewDir, vec3 normal, vec2 texCoord)
{
	//计算环境光
	vec3 ambient = texture2D(material.diffuse, texCoord).rgb * light.ambient;

	//计算漫反射
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * light.diffuse * texture2D(material.diffuse, texCoord).rgb;

	//计算镜面反射
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = spec * light.specular * texture2D(material.specular, texCoord).rgb;

	return ambient + diffuse + specular;
}

vec3 point_lighting(Light light, Material material, vec3 viewDir, vec3 fragPos, vec3 normal, vec2 texCoord)
{
	//计算环境光
	vec3 ambient = texture2D(material.diffuse, texCoord).rgb * light.ambient;

	//计算漫反射
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * light.diffuse * texture2D(material.diffuse, texCoord).rgb;

	//计算镜面反射
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = spec * light.specular * texture2D(material.specular, texCoord).rgb;

	//计算衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + 
				    light.quadratic * distance * distance);

	return (ambient + diffuse + specular) * attenuation;
}

vec3 spot_lighting(Light light, Material material, vec3 viewDir, vec3 fragPos, vec3 normal, vec2 texCoord)
{
	//计算环境光
	vec3 ambient = texture2D(material.diffuse, texCoord).rgb * light.ambient;

	//计算在内圆锥和外圆锥的光强
	vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

	//计算漫反射
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = intensity * diff * light.diffuse * texture2D(material.diffuse, texCoord).rgb;

	//计算镜面反射
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = intensity * spec * light.specular * texture2D(material.specular, texCoord).rgb;

	//计算衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + 
				    light.quadratic * distance * distance);
	return (ambient + diffuse + specular) * attenuation;
}