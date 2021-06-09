#version 460 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;

smooth out vec3 ioVertexColor;
smooth out vec2 ioVertexTexCoord;

uniform mat4 PVM;

void main()
{
	gl_Position = PVM * vec4(vertexPosition, 1.0);
	ioVertexTexCoord = vertexTexCoord;
}