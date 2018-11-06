#version 330 core

in vec2 texCoord;
in vec3 color;
out vec4 fragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main(void)
{
	vec4 tempColor = mix(texture2D(texture1, texCoord), texture2D(texture2, texCoord), 0.1);
	//可以加上颜色尝试一下~
	//fragColor = tempColor * vec4(color, 1.0);
	fragColor = tempColor;
}