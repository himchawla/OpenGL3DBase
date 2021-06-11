#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out VS_GS_VERTEX
{
	out vec4 position;
	out vec3 color;
	out mat4 mvp;
} vs_out;

uniform mat4 PVM;


void main()
{
	gl_Position = PVM * vec4(position, 1.0f);
	vs_out.color = color;
	vs_out.position = gl_Position;
	vs_out.mvp = PVM;
}