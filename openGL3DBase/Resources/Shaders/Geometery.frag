#version 460 core

uniform sampler2D gSampler;

in vec3 outColor;
out vec4 color;

void main()
{
	gSampler;
	color = vec4(outColor, 1.0f);
}