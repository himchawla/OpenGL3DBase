#version 430 core

layout(location = 0) in vec3 vertexPosition;

layout(location = 1) in vec2 vertexTexCoord;

uniform mat4 MVP;
smooth out vec2 fragCoord;

void main()
{
	
    	
	gl_Position = MVP * vec4(vertexPosition, 1.0f);
	fragCoord = vertexPosition.xy * 0.5 + 0.5;
}