#version 460 core

uniform sampler2D gSampler;

in vec3 outColor;
out vec4 color;

void main()
{
	gSampler;
	color = vec4(1.0f,0.0f,0.0f, 1.0f);
}