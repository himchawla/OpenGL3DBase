#version 460 core

out vec4 color;

in float lifetime;

void main()
{
	color = vec4(1.0f, 1.0f, 1.0f, lifetime);
}