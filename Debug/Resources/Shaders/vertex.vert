#version 460 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;

smooth out vec2 ioVertexTexCoord;
smooth out vec3 ioVertexNormal;
smooth out float ioHeight;

uniform mat4 PVM;
uniform mat3 normalMatrix;

void main()
{
	gl_Position = PVM * vec4(vertexPosition, 1.0);
	
	ioVertexTexCoord = vertexTexCoord;
    ioVertexNormal = normalMatrix * vertexNormal;
    ioHeight = vertexPosition.y;
}