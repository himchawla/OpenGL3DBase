#version 460 core

layout (location = 0) in vec4 position;

out float lifetime;
void main()
{
	gl_Position = vec4(position.xyz, 1.0f);
	lifetime = position.w;

} 